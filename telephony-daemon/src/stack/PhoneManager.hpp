#ifndef PHONEMANAGER_HPP
#define PHONEMANAGER_HPP

#include "../common/Looper.hpp"
#include "../common/Handler.hpp"
#include "../common/Message.hpp"
#include <memory>
#include <string>

class Phone;

class PhoneManager : public Looper, public Handler {
public:
    PhoneManager(IQueue<Message>* queue, Phone* phone, int slotId)
        : Looper(queue), phone_(phone), slotId_(slotId) {}

    virtual ~PhoneManager() = default;

    void sendMessage(Message msg) override { getQueue()->push(std::move(msg)); }

    int getSlotId() const { return slotId_; }
    Phone* getPhone() const { return phone_; }

protected:
    virtual void onProcess(Message msg) override {
        handleMessage(std::move(msg));
    }

    Phone* phone_;
    int slotId_;
};

#endif // PHONEMANAGER_HPP