#ifndef VOICECALLTELEREQUEST_HPP
#define VOICECALLTELEREQUEST_HPP

#include "../../common/Event.hpp"
#include <string>

class VoiceCallTeleRequest : public Event {
public:
    VoiceCallTeleRequest(std::string eventId, std::string number)
        : number(std::move(number)) {
        this->eventId = std::move(eventId);
    }

    EventType type() const override {
        return EventType::TELE_REQ;
    }

    std::string debugString() const override {
        return "VoiceCallTeleRequest{eventId=" + eventId + ", number=" + number + "}";
    }

    std::string number;
};

#endif