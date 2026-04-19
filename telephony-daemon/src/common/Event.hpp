#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>

enum class EventType {
    DBUS_REQ,
    DBUS_RES,
    TELE_REQ,
    TELE_RES,
    RIL_REQ,
    RIL_RES
};

class Event {
public:
    virtual ~Event() = default;
    virtual EventType type() const = 0;
    virtual std::string debugString() const = 0;

    std::string eventId;
};

#endif