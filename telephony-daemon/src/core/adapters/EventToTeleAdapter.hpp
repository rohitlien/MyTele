#ifndef EVENTTOTELEADAPTER_HPP
#define EVENTTOTELEADAPTER_HPP

#include "../../common/Event.hpp"
#include "../../delivery/models/VoiceCallDbusRequest.hpp"
#include "../../delivery/models/SmsDbusRequest.hpp"
#include "../../stack/models/VoiceCallTeleRequest.hpp"
#include "../../stack/models/SmsTeleRequest.hpp"
#include <memory>

class EventToTeleAdapter {
public:
    static std::shared_ptr<Event> adapt(const std::shared_ptr<Event>& event) {
        if (event->type() != EventType::DBUS_REQ) {
            return event;
        }

        if (auto callReq = std::dynamic_pointer_cast<VoiceCallDbusRequest>(event)) {
            auto teleEvent = std::make_shared<VoiceCallTeleRequest>(callReq->eventId, callReq->number);
            teleEvent->eventId = callReq->eventId;
            return teleEvent;
        }

        if (auto smsReq = std::dynamic_pointer_cast<SmsDbusRequest>(event)) {
            auto teleEvent = std::make_shared<SmsTeleRequest>(smsReq->eventId, smsReq->number, smsReq->message);
            teleEvent->eventId = smsReq->eventId;
            return teleEvent;
        }

        return event;
    }
};

#endif