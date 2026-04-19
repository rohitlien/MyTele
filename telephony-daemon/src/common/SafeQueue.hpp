#ifndef SAFEQUEUE_HPP
#define SAFEQUEUE_HPP

#include "IQueue.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>

// A simple Thread-Safe Queue
template <typename T>
class SafeQueue : public IQueue<T> {
    std::queue<T> q;
    std::mutex m;
    std::condition_variable cv;
public:
    void push(T val) override {
        std::lock_guard<std::mutex> lock(m);
        q.push(std::move(val));
        cv.notify_one();
    }

    T pop() override {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this] { return !q.empty(); });
        T val = std::move(q.front());
        q.pop();
        return val;
    }
};

#endif