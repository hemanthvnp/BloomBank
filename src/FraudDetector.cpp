#include "bloombank/FraudDetector.hpp"

#include <iostream>

namespace bloombank {

bool FraudDetector::isLargeTransaction(double amount) const {
    return amount >= kLargeAmountThreshold;
}

bool FraudDetector::isFrequentTransaction(int accountId) const {
    std::time_t now = std::time(nullptr);
    int count = 0;
    for (const auto& record : history_.at(accountId)) {
        if (std::difftime(now, record.second) <= 60) {
            ++count;
        }
    }
    return count >= kMaxTransactionsPerMinute;
}

bool FraudDetector::hasUnusualFrequency(int accountId) const {
    const auto& records = history_.at(accountId);
    if (records.size() < 5) {
        return false;
    }

    double totalInterval = 0;
    for (std::size_t i = 1; i < records.size(); ++i) {
        totalInterval += std::difftime(records[i].second, records[i - 1].second);
    }
    double avgInterval = totalInterval / static_cast<double>(records.size() - 1);
    return avgInterval < kUnusualFrequencyThresholdSeconds;
}

std::vector<std::string> FraudDetector::recordTransaction(int accountId, double amount) {
    std::time_t now = std::time(nullptr);
    history_[accountId].push_back({amount, now});

    std::vector<std::string> alerts;
    if (isLargeTransaction(amount)) {
        alerts.push_back("ALERT: Large transaction detected for account #" + std::to_string(accountId) +
                          " - Amount: " + std::to_string(amount));
    }
    if (isFrequentTransaction(accountId)) {
        alerts.push_back("ALERT: High transaction frequency detected for account #" + std::to_string(accountId));
    }
    if (hasUnusualFrequency(accountId)) {
        alerts.push_back("ALERT: Unusually frequent transactions detected for account #" + std::to_string(accountId));
    }

    if (!alerts.empty()) {
        flaggedAccounts_.add(accountId);
    }
    return alerts;
}

bool FraudDetector::possiblyFlagged(int accountId) const {
    return flaggedAccounts_.possiblyContains(accountId);
}

void FraudDetector::displaySuspiciousTransactions(int accountId) const {
    std::cout << "\nSuspicious Activity Report for Account #" << accountId << std::endl;

    auto it = history_.find(accountId);
    if (it == history_.end()) {
        std::cout << "No transaction records found for this account." << std::endl;
        return;
    }

    bool suspiciousFound = false;
    for (const auto& record : it->second) {
        if (record.first >= kLargeAmountThreshold) {
            std::cout << "Large Transaction: " << record.first << " at " << std::ctime(&record.second);
            suspiciousFound = true;
        }
    }
    if (!suspiciousFound) {
        std::cout << "No suspicious transactions detected." << std::endl;
    }
}

} // namespace bloombank
