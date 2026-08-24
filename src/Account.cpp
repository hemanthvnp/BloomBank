#include "bloombank/Account.hpp"

#include <iomanip>
#include <iostream>

namespace bloombank {

Account::Account(int id, std::string name, double initialBalance)
    : id_(id), name_(std::move(name)), balance_(initialBalance), active_(true), creationDate_(std::time(nullptr)) {}

bool Account::deposit(double amount) {
    if (amount <= 0) {
        return false;
    }
    balance_ += amount;
    return true;
}

bool Account::withdraw(double amount) {
    if (amount <= 0 || amount > balance_) {
        return false;
    }
    balance_ -= amount;
    return true;
}

void Account::displayInfo() const {
    std::cout << "Account #" << id_ << ":" << std::endl;
    std::cout << "  Customer: " << name_ << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Balance: Rs. " << balance_ << std::endl;
    std::cout << "  Status: " << (active_ ? "Active" : "Inactive") << std::endl;

    char dateStr[32];
    std::strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", std::localtime(&creationDate_));
    std::cout << "  Created on: " << dateStr << std::endl;
}

} // namespace bloombank
