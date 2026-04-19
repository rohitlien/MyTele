#ifndef EVENTMANAGERQUEUE_HPP
#define EVENTMANAGERQUEUE_HPP

#include "../common/IQueue.hpp"
#include "../common/Message.hpp"
#include "../common/SafeQueue.hpp"
#include <iostream>

class EventManagerQueue : public SafeQueue<Message> {
public:
    void push(Message val) override {
        std::string eventId = val.event ? val.event->eventId : "<none>";
        std::cout << "[EventManagerQueue::push] Processing message event ID: " << eventId 
                  << " - Has reply handler: " << (val.replyHandler != nullptr ? "Yes" : "No") << std::endl;
        SafeQueue<Message>::push(std::move(val));
    }

    Message pop() override {
        std::cout << "[EventManagerQueue::pop] Fetching message for event logic..." << std::endl;
        Message msg = SafeQueue<Message>::pop();
        std::string eventId = msg.event ? msg.event->eventId : "<none>";
        std::cout << "[EventManagerQueue::pop] Processing message event ID: " << eventId << std::endl;
        return msg;
    }
};

#endif