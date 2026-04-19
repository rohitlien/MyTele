#ifndef VOICECALLDBUSREQUEST_HPP
#define VOICECALLDBUSREQUEST_HPP

#include "../../common/Event.hpp"
#include <string>

class VoiceCallDbusRequest : public Event {
public:
    VoiceCallDbusRequest(std::string serial, std::string number)
        : dbusSerial(std::move(serial)), number(std::move(number)) {}

    EventType type() const override {
        return EventType::DBUS_REQ;
    }

    std::string debugString() const override {
        return "VoiceCallDbusRequest{serial=" + dbusSerial + ", number=" + number + "}";
    }

    std::string dbusSerial;
    std::string number;
};

#endif