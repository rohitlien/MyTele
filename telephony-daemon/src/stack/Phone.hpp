#ifndef PHONE_HPP
#define PHONE_HPP

#include "../common/CallManager.hpp"
#include "SmsManager.hpp"
#include "../common/DataManager.hpp"
#include "../common/IQueue.hpp"
#include "../common/SafeQueue.hpp"
#include "../common/Registrant.hpp"
#include <memory>
#include <iostream>
#include <unordered_map>
#include <string>

enum class ModemState {
    OFF,
    ON,
    READY
};

enum class SimState {
    ABSENT,
    PIN_REQUIRED,
    PUK_REQUIRED,
    NETWORK_LOCKED,
    READY,
    NOT_READY,
    PERM_DISABLED,
    CARD_IO_ERROR,
    CARD_RESTRICTED
};

class Phone {
public:
    Phone(int slotId)
        : slotId_(slotId),
          callManagerQueue_(std::make_unique<SafeQueue<Message>>()),
          smsManagerQueue_(std::make_unique<SafeQueue<Message>>()),
          dataManagerQueue_(std::make_unique<SafeQueue<Message>>()),
          modemState_(ModemState::OFF),
          simState_(SimState::ABSENT) {

        // Create managers
        callManager_ = std::make_unique<CallManager>(callManagerQueue_.get(), this, slotId_);
        smsManager_ = std::make_unique<SmsManager>(smsManagerQueue_.get(), this, slotId_);
        
        // Create DataManager with a callback to register for SIM state changes
        dataManager_ = std::make_unique<DataManager>(dataManagerQueue_.get(), this, slotId_,
            [this](std::shared_ptr<Registrant> registrant) {
                this->registerForStateChanges("sim_state", registrant);
            },
            [this](std::shared_ptr<Registrant> registrant) {
                this->unregisterForStateChanges("sim_state", registrant);
            });

        std::cout << "[Phone Slot " << slotId_ << "] Created with managers" << std::endl;
    }

    ~Phone() {
        stop();
    }

    void start() {
        callManager_->start();
        smsManager_->start();
        dataManager_->start();
        std::cout << "[Phone Slot " << slotId_ << "] Started all managers" << std::endl;
    }

    void stop() {
        if (callManager_) callManager_->stop();
        if (smsManager_) smsManager_->stop();
        if (dataManager_) dataManager_->stop();
        std::cout << "[Phone Slot " << slotId_ << "] Stopped all managers" << std::endl;
    }

    int getSlotId() const { return slotId_; }

    CallManager* getCallManager() { return callManager_.get(); }
    SmsManager* getSmsManager() { return smsManager_.get(); }
    DataManager* getDataManager() { return dataManager_.get(); }

    // Modem state management
    void setModemState(ModemState state) {
        modemState_ = state;
        std::string key = "modem_state";
        std::string value = modemStateToString(state);
        getRegistrantList(key).notifyAll(key, value);
    }

    ModemState getModemState() const { return modemState_; }

    // SIM state management
    void setSimState(SimState state) {
        simState_ = state;
        std::string key = "sim_state";
        std::string value = simStateToString(state);
        getRegistrantList(key).notifyAll(key, value);
    }

    SimState getSimState() const { return simState_; }

    // Generic state management
    void setState(const std::string& key, const std::string& value) {
        stateStore_[key] = value;
        getRegistrantList(key).notifyAll(key, value);
    }

    std::string getState(const std::string& key) const {
        auto it = stateStore_.find(key);
        return (it != stateStore_.end()) ? it->second : "";
    }

    // Registrant management
    void registerForStateChanges(const std::string& key, std::shared_ptr<Registrant> registrant) {
        getRegistrantList(key).add(registrant);
    }

    void unregisterForStateChanges(const std::string& key, std::shared_ptr<Registrant> registrant) {
        getRegistrantList(key).remove(registrant);
    }

private:
    RegistrantList& getRegistrantList(const std::string& key) {
        return registrantLists_[key];
    }

    static std::string modemStateToString(ModemState state) {
        switch (state) {
            case ModemState::OFF: return "OFF";
            case ModemState::ON: return "ON";
            case ModemState::READY: return "READY";
            default: return "UNKNOWN";
        }
    }

    static std::string simStateToString(SimState state) {
        switch (state) {
            case SimState::ABSENT: return "ABSENT";
            case SimState::PIN_REQUIRED: return "PIN_REQUIRED";
            case SimState::PUK_REQUIRED: return "PUK_REQUIRED";
            case SimState::NETWORK_LOCKED: return "NETWORK_LOCKED";
            case SimState::READY: return "READY";
            case SimState::NOT_READY: return "NOT_READY";
            case SimState::PERM_DISABLED: return "PERM_DISABLED";
            case SimState::CARD_IO_ERROR: return "CARD_IO_ERROR";
            case SimState::CARD_RESTRICTED: return "CARD_RESTRICTED";
            default: return "UNKNOWN";
        }
    }

    int slotId_;
    std::unique_ptr<CallManager> callManager_;
    std::unique_ptr<SmsManager> smsManager_;
    std::unique_ptr<DataManager> dataManager_;

    std::unique_ptr<IQueue<Message>> callManagerQueue_;
    std::unique_ptr<IQueue<Message>> smsManagerQueue_;
    std::unique_ptr<IQueue<Message>> dataManagerQueue_;

    // State management
    ModemState modemState_;
    SimState simState_;
    std::unordered_map<std::string, std::string> stateStore_;
    std::unordered_map<std::string, RegistrantList> registrantLists_;
};

#endif // PHONE_HPP