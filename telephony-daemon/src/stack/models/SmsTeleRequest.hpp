#ifndef SMSTELEREQUEST_HPP
#define SMSTELEREQUEST_HPP

#include "../../common/Event.hpp"
#include <string>

class SmsTeleRequest : public Event {
public:
    SmsTeleRequest(std::string eventId, std::string number, std::string message)
<<<<<<< HEAD
        : number(std::move(number)), message(std::move(message)) {
=======
        : number(std::move(number)), message(std::move(message)), controller_type("sms") {
>>>>>>> ee7bd08 (First Commit)
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
<<<<<<< HEAD
=======
    std::string controller_type;
>>>>>>> ee7bd08 (First Commit)
};

#endif