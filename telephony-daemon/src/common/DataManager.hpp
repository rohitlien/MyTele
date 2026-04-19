#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include "../stack/PhoneManager.hpp"
#include "../common/Registrant.hpp"
#include <iostream>
#include <memory>

class DataManager : public PhoneManager {
public:
    using RegistrantCallback = std::function<void(std::shared_ptr<Registrant>)>;

    DataManager(IQueue<Message>* queue, Phone* phone, int slotId,
                RegistrantCallback registerCallback, RegistrantCallback unregisterCallback)
        : PhoneManager(queue, phone, slotId),
          registerCallback_(registerCallback),
          unregisterCallback_(unregisterCallback) {
        // Register for SIM state changes with the phone via callback
        simStateRegistrant_ = std::make_shared<Registrant>(
            [this](const std::string& key, const std::string& value) {
                this->onSimStateChanged(key, value);
            }
        );
        registerCallback_(simStateRegistrant_);
    }

    ~DataManager() override {
        // Unregister when destroyed
        unregisterCallback_(simStateRegistrant_);
    }

    void handleMessage(Message msg) override {
        if (!msg.event) {
            std::cout << "[DataManager Slot " << slotId_ << "] Received empty message, skipping." << std::endl;
            return;
        }

        std::cout << "[DataManager Slot " << slotId_ << "] Processing event: " << msg.event->debugString() << std::endl;

        // Handle data-related events
        if (msg.event->type() == EventType::TELE_REQ) {
            // Process data requests
            std::cout << "[DataManager Slot " << slotId_ << "] Handling data request" << std::endl;
        }
    }

private:
    void onSimStateChanged(const std::string& key, const std::string& value) {
        std::cout << "[DataManager Slot " << slotId_ << "] SIM state changed: " << key << " = " << value << std::endl;

        // React to SIM state changes - e.g., enable/disable data services
        if (value == "READY") {
            std::cout << "[DataManager Slot " << slotId_ << "] SIM ready - enabling data services" << std::endl;
        } else if (value == "ABSENT") {
            std::cout << "[DataManager Slot " << slotId_ << "] SIM absent - disabling data services" << std::endl;
        }
    }

    std::shared_ptr<Registrant> simStateRegistrant_;
    RegistrantCallback registerCallback_;
    RegistrantCallback unregisterCallback_;
};

#endif // DATAMANAGER_HPP