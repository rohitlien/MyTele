#ifndef SMSMANAGER_HPP
#define SMSMANAGER_HPP

#include "../PhoneManager.hpp"
#include "../models/SmsTeleRequest.hpp"
#include <iostream>

class SmsManager : public PhoneManager {
public:
    SmsManager(Phone* phone, int slotId)
        : PhoneManager(phone, slotId) {}

    void sendSms(SmsTeleRequest* smsReq) {
        std::cout << "[SmsManager Slot " << slotId_ << "] Processing SMS request: " << smsReq->debugString() << std::endl;
        std::cout << "[SmsManager Slot " << slotId_ << "] Sending SMS to " << smsReq->number << " with message: " << smsReq->message << std::endl;
    }

    void handleMessage(Message msg) override {
        // Deprecated: kept for backward compatibility
    }
};

#endif // SMSMANAGER_HPP