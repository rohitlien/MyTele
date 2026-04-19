#ifndef TELEPHONYEVENT_HPP
#define TELEPHONYEVENT_HPP

#include <string>

enum class EventType {
    DBUS_REQUEST,
    // Add more
};

struct TelephonyEvent {
    EventType type;
    std::string command;
    std::string data;
};

#endif