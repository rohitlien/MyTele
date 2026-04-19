#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include "../common/Looper.hpp"
#include "../common/Handler.hpp"
#include "../common/Message.hpp"
#include "adapters/EventToTeleAdapter.hpp"
#include <iostream>
#include <map>
#include <memory>
#include <atomic>

class EventManager : public Looper, public Handler {
public:
    EventManager(IQueue<Message>* queue, Handler* teleStackHandler, Handler* dbusHandler)
        : Looper(queue), m_teleStack(teleStackHandler), m_dbus(dbusHandler), m_event_counter(0) {}

    void sendMessage(Message msg) override { getQueue()->push(std::move(msg)); }

protected:
    void onProcess(Message msg) override {
        handleMessage(std::move(msg));
    }

    void handleMessage(Message msg) override {
        if (!msg.event) {
            std::cout << "[EventManager] Received empty message, skipping." << std::endl;
            return;
        }

        auto event = msg.event;
        std::cout << "[EventManager] Logic Layer received event: " << event->debugString() << std::endl;

        if (event->type() == EventType::DBUS_REQ ||
            event->type() == EventType::TELE_REQ ||
            event->type() == EventType::RIL_REQ) {
            event->eventId = generateEventId();
            m_eventStore[event->eventId] = event;
            std::cout << "[EventManager] Stored event with id: " << event->eventId << std::endl;
        }

        auto teleEvent = EventToTeleAdapter::adapt(event);
        teleEvent->eventId = event->eventId;

        Message teleMsg(teleEvent, nullptr);
        std::cout << "  -> Forwarding event id " << teleEvent->eventId << " to TeleStack" << std::endl;
        m_teleStack->sendMessage(std::move(teleMsg));

        if (event->type() == EventType::DBUS_REQ && msg.replyHandler) {
            std::cout << "[EventManager] Reply decision taken for DBUS request." << std::endl;
            msg.replyHandler->sendReply("ACCEPTED");
        }
    }

private:
    std::string generateEventId() {
        return "event_" + std::to_string(m_event_counter.fetch_add(1));
    }

    Handler* m_teleStack;
    Handler* m_dbus;
    std::map<std::string, std::shared_ptr<Event>> m_eventStore;
    std::atomic<int> m_event_counter;
};

#endif