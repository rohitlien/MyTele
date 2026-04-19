#ifndef LOOPER_HPP
#define LOOPER_HPP

#include "IQueue.hpp"
#include "Message.hpp"
#include <thread>

class Looper {
public:
    Looper(IQueue<Message>* queue) : m_queue(queue), m_running(false) {}
    virtual ~Looper() { stop(); }

    void start() {
        m_running = true;
        m_thread = std::thread(&Looper::run, this);
    }

    void stop() {
        m_running = false;
        // In a real app, push a "QUIT" message to break the pop() wait
        if (m_thread.joinable()) m_thread.detach(); 
    }

    IQueue<Message>* getQueue() { return m_queue; }

protected:
    virtual void onProcess(Message msg) = 0;

private:
    void run() {
        while (m_running) {
            Message msg = m_queue->pop();
            onProcess(std::move(msg));
        }
    }
    IQueue<Message>* m_queue;
    std::thread m_thread;
    bool m_running;
};

#endif