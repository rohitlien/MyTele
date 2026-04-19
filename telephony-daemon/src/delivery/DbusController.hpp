#ifndef DBUSCONTROLLER_HPP
#define DBUSCONTROLLER_HPP

#include "../common/Handler.hpp"
#include "../common/Message.hpp"
#include "adapters/CallAdapter.hpp"
#include "adapters/SmsAdapter.hpp"
#include "adapters/DbusReplyHandler.hpp"
#include <thread>
#include <memory>
#include <iostream>
#include <chrono>
#include <atomic>

class DbusController {
public:
    DbusController(Handler* eventMgrHandler) : m_eventMgr(eventMgrHandler), m_serial_counter(0) {}

    void start() {
        m_thread = std::thread([this]() {
            std::cout << "[DbusController] Listening for D-Bus requests..." << std::endl;
            
            // Simulate D-Bus method calls with serials
            std::this_thread::sleep_for(std::chrono::seconds(1));
            
            // Simulate Call method
            std::string serial1 = generateSerial();
            auto replyHandler1 = std::make_shared<DbusReplyHandler>(serial1, "Call Request");
            auto event1 = CallAdapter::adapt(serial1, "+91987654321");
            Message m1(event1, replyHandler1);
            m_eventMgr->sendMessage(std::move(m1));

            std::this_thread::sleep_for(std::chrono::seconds(1));
            
            // Simulate Sms method
            std::string serial2 = generateSerial();
            auto replyHandler2 = std::make_shared<DbusReplyHandler>(serial2, "SMS Request");
            auto event2 = SmsAdapter::adapt(serial2, "+91987654321", "Hello World");
            Message m2(event2, replyHandler2);
            m_eventMgr->sendMessage(std::move(m2));
        });
    }

    void join() { if (m_thread.joinable()) m_thread.join(); }

private:
    std::string generateSerial() {
        return "serial_" + std::to_string(m_serial_counter.fetch_add(1));
    }

    std::thread m_thread;
    Handler* m_eventMgr;
    std::atomic<int> m_serial_counter;
};

#endif