#ifndef SMSDBUSREQUEST_HPP
#define SMSDBUSREQUEST_HPP

#include "../../common/Event.hpp"
#include <string>

class SmsDbusRequest : public Event {
public:
    SmsDbusRequest(std::string serial, std::string number, std::string message)
        : dbusSerial(std::move(serial)), number(std::move(number)), message(std::move(message)) {}

    EventType type() const override {
        return EventType::DBUS_REQ;
    }

    std::string debugString() const override {
        return "SmsDbusRequest{serial=" + dbusSerial + ", number=" + number + ", message=" + message + "}";
    }

    std::string dbusSerial;
    std::string number;
    std::string message;
};

#endif