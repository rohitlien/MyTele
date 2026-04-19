#ifndef SMSTELEREQUEST_HPP
#define SMSTELEREQUEST_HPP

#include "../../common/Event.hpp"
#include <string>

class SmsTeleRequest : public Event {
public:
    SmsTeleRequest(std::string eventId, std::string number, std::string message)
        : number(std::move(number)), message(std::move(message)) {
        this->eventId = std::move(eventId);
    }

    EventType type() const override {
        return EventType::TELE_REQ;
    }

    std::string debugString() const override {
        return "SmsTeleRequest{eventId=" + eventId + ", number=" + number + ", message=" + message + "}";
    }

    std::string number;
    std::string message;
};

#endif