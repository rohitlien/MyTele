#ifndef CALLMANAGER_HPP
#define CALLMANAGER_HPP

#include "../stack/PhoneManager.hpp"
#include <iostream>

class CallManager : public PhoneManager {
public:
    CallManager(IQueue<Message>* queue, Phone* phone, int slotId)
        : PhoneManager(queue, phone, slotId) {}

    void handleMessage(Message msg) override {
        if (!msg.event) {
            std::cout << "[CallManager Slot " << slotId_ << "] Received empty message, skipping." << std::endl;
            return;
        }

        std::cout << "[CallManager Slot " << slotId_ << "] Processing event: " << msg.event->debugString() << std::endl;

        // Handle call-related events
        if (msg.event->type() == EventType::TELE_REQ) {
            // Process voice call requests
            std::cout << "[CallManager Slot " << slotId_ << "] Handling voice call request" << std::endl;
        }
    }
};

#endif // CALLMANAGER_HPP