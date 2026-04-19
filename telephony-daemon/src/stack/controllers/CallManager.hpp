#ifndef CALLMANAGER_HPP
#define CALLMANAGER_HPP

#include "../PhoneManager.hpp"
#include "../models/VoiceCallTeleRequest.hpp"
#include <iostream>

class CallManager : public PhoneManager {
public:
    CallManager(Phone* phone, int slotId)
        : PhoneManager(phone, slotId) {}

    void sendCall(VoiceCallTeleRequest* callReq) {
        std::cout << "[CallManager Slot " << slotId_ << "] Processing voice call request: " << callReq->debugString() << std::endl;
        std::cout << "[CallManager Slot " << slotId_ << "] Dialing " << callReq->number << std::endl;
    }

    void handleMessage(Message msg) override {
        // Deprecated: kept for backward compatibility
    }
};

#endif // CALLMANAGER_HPP