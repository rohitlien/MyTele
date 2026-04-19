#ifndef TELESTACKTHREADSECONDARYQUEUE_HPP
#define TELESTACKTHREADSECONDARYQUEUE_HPP

#include "../common/IQueue.hpp"
#include "../common/Message.hpp"
#include "../common/SafeQueue.hpp"
#include <iostream>

class TeleStackThreadSecondaryQueue : public SafeQueue<Message> {
public:
    void push(Message val) override {
        std::string eventId = val.event ? val.event->eventId : "<none>";
        std::cout << "[TeleStackThreadSecondaryQueue::push] Adding internal message with event ID: " << eventId << std::endl;
        SafeQueue<Message>::push(std::move(val));
    }

    Message pop() override {
        std::cout << "[TeleStackThreadSecondaryQueue::pop] Retrieving internal message from secondary queue..." << std::endl;
        Message msg = SafeQueue<Message>::pop();
        std::string eventId = msg.event ? msg.event->eventId : "<none>";
        std::cout << "[TeleStackThreadSecondaryQueue::pop] Retrieved internal message event ID: " << eventId << std::endl;
        return msg;
    }
};

#endif // TELESTACKTHREADSECONDARYQUEUE_HPP