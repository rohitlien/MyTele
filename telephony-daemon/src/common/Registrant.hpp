#ifndef REGISTRANT_HPP
#define REGISTRANT_HPP

#include <memory>
#include <vector>
#include <functional>
#include <mutex>
#include <string>

// Android-style Registrant for callback notifications
class Registrant {
public:
    using Callback = std::function<void(const std::string& key, const std::string& value)>;

    Registrant(Callback cb) : callback(cb) {}
    void notify(const std::string& key, const std::string& value) {
        if (callback) callback(key, value);
    }

private:
    Callback callback;
};

class RegistrantList {
public:
    void add(std::shared_ptr<Registrant> registrant) {
        std::lock_guard<std::mutex> lock(mutex_);
        registrants_.push_back(registrant);
    }

    void remove(std::shared_ptr<Registrant> registrant) {
        std::lock_guard<std::mutex> lock(mutex_);
        registrants_.erase(
            std::remove(registrants_.begin(), registrants_.end(), registrant),
            registrants_.end()
        );
    }

    void notifyAll(const std::string& key, const std::string& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& registrant : registrants_) {
            registrant->notify(key, value);
        }
    }

private:
    std::vector<std::shared_ptr<Registrant>> registrants_;
    std::mutex mutex_;
};

#endif // REGISTRANT_HPP