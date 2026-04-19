#ifndef SMSMANAGER_HPP
#define SMSMANAGER_HPP

#include "PhoneManager.hpp"
#include <iostream>

class SmsManager : public PhoneManager {
public:
    SmsManager(IQueue<Message>* queue, Phone* phone, int slotId)
        : PhoneManager(queue, phone, slotId) {}

    void handleMessage(Message msg) override {
        if (!msg.event) {
            std::cout << "[SmsManager Slot " << slotId_ << "] Received empty message, skipping." << std::endl;
            return;
        }

        std::cout << "[SmsManager Slot " << slotId_ << "] Processing event: " << msg.event->debugString() << std::endl;

        // Handle SMS-related events
        if (msg.event->type() == EventType::TELE_REQ) {
            // Process SMS requests
            std::cout << "[SmsManager Slot " << slotId_ << "] Handling SMS request" << std::endl;
        }
    }
};

#endif // SMSMANAGER_HPP