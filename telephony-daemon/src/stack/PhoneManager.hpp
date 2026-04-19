#ifndef PHONEMANAGER_HPP
#define PHONEMANAGER_HPP

#include "../common/Handler.hpp"
#include "../common/Message.hpp"
#include <memory>
#include <string>

class Phone;

class PhoneManager : public Handler {
public:
    PhoneManager(Phone* phone, int slotId)
        : phone_(phone), slotId_(slotId) {}

    virtual ~PhoneManager() = default;

    void sendMessage(Message msg) override {
        // Deprecated: no longer used with direct method calls
    }

    virtual void handleMessage(Message msg) {
        // Deprecated: kept for backward compatibility
    }

    int getSlotId() const { return slotId_; }
    Phone* getPhone() const { return phone_; }

protected:
    Phone* phone_;
    int slotId_;
};

#endif // PHONEMANAGER_HPP