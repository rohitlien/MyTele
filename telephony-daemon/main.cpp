#include <iostream>
#include <thread>
#include <memory>
#include <chrono>

// Includes from packages
#include "src/common/Message.hpp"
#include "src/common/Handler.hpp"
#include "src/common/IQueue.hpp"
#include "src/common/SafeQueue.hpp"
#include "src/common/Looper.hpp"
#include "src/stack/TeleStackThread.hpp"
#include "src/stack/TeleStackThreadMainQueue.hpp"
#include "src/stack/TeleStackThreadSecondaryQueue.hpp"
#include "src/core/EventManager.hpp"
#include "src/core/EventManagerQueue.hpp"
#include "src/delivery/DbusController.hpp"
#include "src/delivery/DbusControllerQueue.hpp"

int main() {
    std::cout << "--- Starting Telephony Framework ---" << std::endl;

    // Create custom queues for TeleStackThread (two queues)
    auto teleStackMainQueue = std::make_unique<TeleStackThreadMainQueue>();
    auto teleStackSecondaryQueue = std::make_unique<TeleStackThreadSecondaryQueue>();
    auto eventMgrQueue = std::make_unique<EventManagerQueue>();
    auto dbusControllerQueue = std::make_unique<DbusControllerQueue>();

    // 1. Initialize TeleStackThread (Lowest Layer) with two queues
    // TeleStackThread now creates Phone objects internally and initializes their states
    auto teleStackThread = std::make_unique<TeleStackThread>(teleStackMainQueue.get(), teleStackSecondaryQueue.get());
    teleStackThread->start();
    teleStackThread->startPhones();

    // 2. Initialize EventManager with custom queue and give it TeleStackThread's Handler
    auto eventMgr = std::make_unique<EventManager>(eventMgrQueue.get(), teleStackThread.get(), nullptr);
    eventMgr->start();

    // 3. Initialize D-Bus Controller
    DbusController dbus(eventMgr.get());
    dbus.start();

    // Demonstrate Phone state management functionality
    teleStackThread->demonstratePhoneRegistry();

    // Keep main alive while the simulation runs
    dbus.join();
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "--- Simulation Finished ---" << std::endl;
    
    return 0;
}