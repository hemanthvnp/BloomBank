#pragma once

#include <ctime>
#include <string>

namespace bloombank {

class Account {
public:
    Account(int id, std::string name, double initialBalance = 0.0);

    int getId() const { return id_; }
    const std::string& getName() const { return name_; }
    double getBalance() const { return balance_; }
    bool isActive() const { return active_; }
    std::time_t getCreationDate() const { return creationDate_; }

    // Returns false (and leaves the balance unchanged) if amount <= 0.
    bool deposit(double amount);
    // Returns false (and leaves the balance unchanged) if amount is
    // non-positive or exceeds the current balance.
    bool withdraw(double amount);

    void activate() { active_ = true; }
    void deactivate() { active_ = false; }

    void displayInfo() const;

private:
    int id_;
    std::string name_;
    double balance_;
    bool active_;
    std::time_t creationDate_;
};

} // namespace bloombank
