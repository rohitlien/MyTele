#ifndef TELESTACKQUEUE_HPP
#define TELESTACKQUEUE_HPP

#include "../common/IQueue.hpp"
#include "../common/Message.hpp"
#include "../common/SafeQueue.hpp"
#include <iostream>

class TeleStackQueue : public SafeQueue<Message> {
public:
    void push(Message val) override {
        std::string eventId = val.event ? val.event->eventId : "<none>";
        std::cout << "[TeleStackQueue::push] Adding message with event ID: " << eventId << std::endl;
        SafeQueue<Message>::push(std::move(val));
    }

    Message pop() override {
        std::cout << "[TeleStackQueue::pop] Retrieving message from queue..." << std::endl;
        Message msg = SafeQueue<Message>::pop();
        std::string eventId = msg.event ? msg.event->eventId : "<none>";
        std::cout << "[TeleStackQueue::pop] Retrieved message event ID: " << eventId << std::endl;
        return msg;
    }
};

#endif