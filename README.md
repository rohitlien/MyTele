# Telephony Stack Architecture

## Overview
This telephony daemon implements a multi-threaded telephony framework with Android-inspired design patterns.

## Key Components

### 1. TeleStackThread
- **Purpose**: Main telephony stack thread that manages phone instances
- **Queues**: 
  - `mainQueue`: Handles events from EventManager
  - `secondaryQueue`: Handles internal events
- **Phones**: Manages two Phone objects (slot 0 and slot 1)

### 2. Phone
- **Purpose**: Represents a single SIM slot/phone instance
- **Managers**:
  - `CallManager`: Handles voice calls
  - `SmsManager`: Handles SMS operations
  - `DataManager`: Handles data connectivity
- **Slot-based**: Each phone has a unique slot ID

### 3. PhoneRegistry
- **Purpose**: Global state management for modem and SIM states
- **Features**:
  - Stores modem states (OFF, ON, READY)
  - Stores SIM states (ABSENT, READY, PIN_REQUIRED, etc.)
  - Thread-safe state management
  - Android-style Registrant pattern for notifications

### 4. Registrants (Android-style)
- **Purpose**: Inter-module communication mechanism
- **Usage**: Components can register callbacks for state changes
- **Example**: DataManager registers for SIM state changes to enable/disable data services

## Architecture Flow
```
DBus Controller → EventManager → TeleStackThread → Phone Managers
                                      ↓
                                PhoneRegistry (global state)
```

## Key Features Implemented
- ✅ TeleStackThread as starting point
- ✅ Two Phone objects with slot-based identification
- ✅ CallManager, SmsManager, DataManager per phone
- ✅ Two queues in TeleStackThread (main + secondary)
- ✅ PhoneRegistry for global state management
- ✅ Android-style Registrants for intermodule communication
- ✅ Thread-safe message passing with Looper/Handler pattern

## Usage Example
```cpp
// Initialize registry
PhoneRegistry::getInstance().setSimState(0, SimState::READY);

// Register for state changes
auto registrant = std::make_shared<Registrant>([](const std::string& key, const std::string& value) {
    std::cout << "State changed: " << key << " = " << value << std::endl;
});
PhoneRegistry::getInstance().registerForStateChanges("sim_state_0", registrant);
```