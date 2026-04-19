#ifndef HANDLER_HPP
#define HANDLER_HPP

#include "Message.hpp"

class Handler {
public:
    virtual ~Handler() = default;
    virtual void handleMessage(Message msg) = 0;
    virtual void sendMessage(Message msg) = 0;
};

#endif