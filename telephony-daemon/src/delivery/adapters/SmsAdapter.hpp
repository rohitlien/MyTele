#ifndef SMSADAPTER_HPP
#define SMSADAPTER_HPP

#include "../models/SmsDbusRequest.hpp"
#include <memory>
#include <string>

class SmsAdapter {
public:
    static std::shared_ptr<Event> adapt(const std::string& serial, const std::string& number, const std::string& message) {
        return std::make_shared<SmsDbusRequest>(serial, number, message);
    }
};

#endif