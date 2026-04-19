#ifndef TELESTACKTHREAD_HPP
#define TELESTACKTHREAD_HPP

#include "../common/Looper.hpp"
#include "../common/Handler.hpp"
#include "../common/Message.hpp"
#include "Phone.hpp"
#include "../common/IQueue.hpp"
#include "../common/SafeQueue.hpp"
#include "models/VoiceCallTeleRequest.hpp"
#include "models/SmsTeleRequest.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>

class TeleStackThread : public Looper, public Handler {
public:
    TeleStackThread(IQueue<Message>* mainQueue, IQueue<Message>* secondaryQueue)
        : Looper(mainQueue), mainQueue_(mainQueue), secondaryQueue_(secondaryQueue) {

        // Create two Phone objects for slot 0 and slot 1
        phones_.push_back(std::make_unique<Phone>(0));
        phones_.push_back(std::make_unique<Phone>(1));

        // Initialize phone states
        for (auto& phone : phones_) {
            phone->setModemState(ModemState::OFF);
            phone->setSimState(SimState::ABSENT);
        }

        std::cout << "[TeleStackThread] Created with 2 phones and initialized states" << std::endl;
    }

    ~TeleStackThread() {
        stopPhones();
    }

    void startPhones() {
        for (auto& phone : phones_) {
            phone->start();
        }
        std::cout << "[TeleStackThread] Started all phones" << std::endl;
    }

    void stopPhones() {
        for (auto& phone : phones_) {
            phone->stop();
        }
        std::cout << "[TeleStackThread] Stopped all phones" << std::endl;
    }

    void sendMessage(Message msg) override {
        // Send to main queue by default
        mainQueue_->push(std::move(msg));
    }

    void sendInternalMessage(Message msg) {
        // Send to secondary queue for internal events
        secondaryQueue_->push(std::move(msg));
    }

    Phone* getPhone(int slotId) {
        if (slotId >= 0 && slotId < static_cast<int>(phones_.size())) {
            return phones_[slotId].get();
        }
        return nullptr;
    }

    // Methods for demonstration
    void demonstratePhoneRegistry() {
        std::cout << "--- Demonstrating Phone State Management ---" << std::endl;
        Phone* phone0 = getPhone(0);
        if (phone0) {
            std::cout << "Initial modem state slot 0: " << (phone0->getModemState() == ModemState::OFF ? "OFF" : "OTHER") << std::endl;
            std::cout << "Initial SIM state slot 0: " << (phone0->getSimState() == SimState::ABSENT ? "ABSENT" : "OTHER") << std::endl;

            // Simulate SIM state change (this will trigger DataManager callbacks)
            std::cout << "Changing SIM state for slot 0 to READY..." << std::endl;
            phone0->setSimState(SimState::READY);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            std::cout << "Changing SIM state for slot 0 to ABSENT..." << std::endl;
            phone0->setSimState(SimState::ABSENT);
        }
    }

protected:
    void onProcess(Message msg) override {
        handleMessage(std::move(msg));
    }

    void handleMessage(Message msg) override {
        if (!msg.event) {
            std::cout << "[TeleStackThread] Received empty message, skipping." << std::endl;
            return;
        }

        std::cout << "[TeleStackThread] Processing event: " << msg.event->debugString() << std::endl;

        if (msg.event->type() == EventType::TELE_REQ) {
            auto teleReq = std::dynamic_pointer_cast<VoiceCallTeleRequest>(msg.event);
            if (teleReq) {
                std::cout << "[TeleStackThread] Routing voice call request for number: " << teleReq->number << std::endl;
                // Route to appropriate phone based on some logic (for now, use slot 0)
                Phone* phone = getPhone(0);
                if (phone && phone->getCallManager()) {
                    Message callMsg(msg.event, nullptr);
                    phone->getCallManager()->sendMessage(std::move(callMsg));
                }
                return;
            }

            auto smsReq = std::dynamic_pointer_cast<SmsTeleRequest>(msg.event);
            if (smsReq) {
                std::cout << "[TeleStackThread] Routing SMS request to: " << smsReq->number << std::endl;
                // Route to appropriate phone based on some logic (for now, use slot 0)
                Phone* phone = getPhone(0);
                if (phone && phone->getSmsManager()) {
                    Message smsMsg(msg.event, nullptr);
                    phone->getSmsManager()->sendMessage(std::move(smsMsg));
                }
                return;
            }
        }

        std::cout << "[TeleStackThread] Unknown event type." << std::endl;
    }

private:
    IQueue<Message>* mainQueue_;
    IQueue<Message>* secondaryQueue_;
    std::vector<std::unique_ptr<Phone>> phones_;
};

#endif // TELESTACKTHREAD_HPP