#ifndef TELESTACK_HPP
#define TELESTACK_HPP

#include "../common/Looper.hpp"
#include "../common/Handler.hpp"
#include "../common/Message.hpp"
#include "models/VoiceCallTeleRequest.hpp"
#include "models/SmsTeleRequest.hpp"
#include <iostream>

class TeleStack : public Looper, public Handler {
public:
    TeleStack(IQueue<Message>* queue) : Looper(queue) {}
    void sendMessage(Message msg) override { getQueue()->push(std::move(msg)); }

protected:
    void onProcess(Message msg) override {
        handleMessage(std::move(msg));
    }

    void handleMessage(Message msg) override {
        if (!msg.event) {
            std::cout << "[TeleStack] Received empty message, skipping." << std::endl;
            return;
        }

        std::cout << "[TeleStack] HW Layer processing event: " << msg.event->debugString() << std::endl;
        if (msg.event->type() == EventType::TELE_REQ) {
            auto teleReq = std::dynamic_pointer_cast<VoiceCallTeleRequest>(msg.event);
            if (teleReq) {
                std::cout << "  -> Executing telephony request for number: " << teleReq->number << std::endl;
                return;
            }

            auto smsReq = std::dynamic_pointer_cast<SmsTeleRequest>(msg.event);
            if (smsReq) {
                std::cout << "  -> Executing SMS telephony request to: " << smsReq->number
                          << " message: " << smsReq->message << std::endl;
                return;
            }
        }

        std::cout << "  -> Unknown event type at TeleStack." << std::endl;
    }
};

#endif