#include <iostream>
#include <thread>
#include <memory>
#include <chrono>

// Includes from packages
#include "common/Message.hpp"
#include "common/Handler.hpp"
#include "common/IQueue.hpp"
#include "common/SafeQueue.hpp"
#include "common/Looper.hpp"
#include "stack/TeleStack.hpp"
#include "manager/EventManager.hpp"
#include "dbus/DbusController.hpp"

int main() {
    std::cout << "--- Starting Telephony Framework ---" << std::endl;

    // Create queues for each thread
    auto teleStackQueue = std::make_unique<SafeQueue<Message>>();
    auto eventMgrQueue = std::make_unique<SafeQueue<Message>>();

    // 1. Initialize TeleStack (Lowest Layer)
    auto teleStack = std::make_unique<TeleStack>(teleStackQueue.get());
    teleStack->start();

    // 2. Initialize EventManager and give it TeleStack's Handler
    auto eventMgr = std::make_unique<EventManager>(eventMgrQueue.get(), teleStack.get());
    eventMgr->start();

    // 3. Initialize D-Bus Controller
    DbusController dbus(eventMgr.get());
    dbus.start();

    // Keep main alive while the simulation runs
    dbus.join();
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "--- Simulation Finished ---" << std::endl;
    
    return 0;
}