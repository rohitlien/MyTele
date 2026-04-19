#ifndef CALLADAPTER_HPP
#define CALLADAPTER_HPP

#include "../models/VoiceCallDbusRequest.hpp"
#include <memory>
#include <string>

class CallAdapter {
public:
    static std::shared_ptr<Event> adapt(const std::string& serial, const std::string& number) {
        return std::make_shared<VoiceCallDbusRequest>(serial, number);
    }
};

#endif