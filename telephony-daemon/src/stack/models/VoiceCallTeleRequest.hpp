#ifndef VOICECALLTELEREQUEST_HPP
#define VOICECALLTELEREQUEST_HPP

#include "../../common/Event.hpp"
#include <string>

class VoiceCallTeleRequest : public Event {
public:
    VoiceCallTeleRequest(std::string eventId, std::string number)
<<<<<<< HEAD
        : number(std::move(number)) {
=======
        : number(std::move(number)), controller_type("call") {
>>>>>>> ee7bd08 (First Commit)
        this->eventId = std::move(eventId);
    }

    EventType type() const override {
        return EventType::TELE_REQ;
    }

    std::string debugString() const override {
        return "VoiceCallTeleRequest{eventId=" + eventId + ", number=" + number + "}";
    }

    std::string number;
<<<<<<< HEAD
=======
    std::string controller_type;
>>>>>>> ee7bd08 (First Commit)
};

#endif