#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "Event.hpp"
#include <memory>
#include <string>

class IReplyHandler;

struct Message {
    std::shared_ptr<Event> event;
    std::shared_ptr<IReplyHandler> replyHandler;

    Message() = default;
    Message(std::shared_ptr<Event> e, std::shared_ptr<IReplyHandler> handler = nullptr)
        : event(std::move(e)), replyHandler(std::move(handler)) {}
};

class IReplyHandler {
public:
    virtual ~IReplyHandler() = default;
    virtual void sendReply(const std::string& result) = 0;
    virtual void sendError(const std::string& error) = 0;
};

#endif