#ifndef DBUSCONTROLLERQUEUE_HPP
#define DBUSCONTROLLERQUEUE_HPP

#include "../common/IQueue.hpp"
#include "../common/Message.hpp"
#include "../common/SafeQueue.hpp"
#include <iostream>

class DbusControllerQueue : public SafeQueue<Message> {
public:
    void push(Message val) override {
        std::string eventId = val.event ? val.event->eventId : "<none>";
        std::cout << "[DbusControllerQueue::push] Queueing D-Bus message event ID: " << eventId << std::endl;
        SafeQueue<Message>::push(std::move(val));
    }

    Message pop() override {
        std::cout << "[DbusControllerQueue::pop] Retrieving queued D-Bus message..." << std::endl;
        Message msg = SafeQueue<Message>::pop();
        std::string eventId = msg.event ? msg.event->eventId : "<none>";
        std::cout << "[DbusControllerQueue::pop] Retrieved D-Bus message event ID: " << eventId << std::endl;
        return msg;
    }
};

#endif