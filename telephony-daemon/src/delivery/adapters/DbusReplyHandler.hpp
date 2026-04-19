#ifndef DBUSREPLYHANDLER_HPP
#define DBUSREPLYHANDLER_HPP

#include "../../common/Message.hpp"
#include <string>
#include <iostream>

class DbusReplyHandler : public IReplyHandler {
private:
    std::string m_serial;
    std::string m_clientInfo;

public:
    DbusReplyHandler(const std::string& serial, const std::string& clientInfo = "")
        : m_serial(serial), m_clientInfo(clientInfo) {}

    void sendReply(const std::string& result) override {
        std::cout << "[DbusReplyHandler] Sending reply for serial: " << m_serial 
                  << " | Client: " << m_clientInfo << " | Result: " << result << std::endl;
        // In real implementation, this would send back through D-Bus
    }

    void sendError(const std::string& error) override {
        std::cout << "[DbusReplyHandler] Sending error for serial: " << m_serial 
                  << " | Client: " << m_clientInfo << " | Error: " << error << std::endl;
        // In real implementation, this would send back through D-Bus
    }

    const std::string& getSerial() const { return m_serial; }
};

#endif