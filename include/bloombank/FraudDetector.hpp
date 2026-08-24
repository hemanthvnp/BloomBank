#pragma once

#include "bloombank/BloomFilter.hpp"

#include <ctime>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace bloombank {

// Flags potentially fraudulent activity using three heuristics:
//  1. A single transaction above a fixed large-amount threshold.
//  2. More than kMaxTransactionsPerMinute transactions in a rolling 60s window.
//  3. An unusually short average interval between the last several transactions.
//
// A Bloom filter records which accounts have ever been flagged, giving an
// O(HashCount) "has this account triggered an alert before" pre-check before
// paying for the O(n) per-account history scan in displaySuspiciousTransactions.
//
// recordTransaction never prints directly -- it returns the alert messages so
// callers (and tests) can decide how to present them.
class FraudDetector {
public:
    static constexpr double kLargeAmountThreshold = 50000.0;
    static constexpr int kMaxTransactionsPerMinute = 5;
    static constexpr double kUnusualFrequencyThresholdSeconds = 3.0;

    std::vector<std::string> recordTransaction(int accountId, double amount);
    bool possiblyFlagged(int accountId) const;
    void displaySuspiciousTransactions(int accountId) const;

private:
    using TransactionRecord = std::pair<double, std::time_t>;

    BloomFilter<1000, 3> flaggedAccounts_;
    std::unordered_map<int, std::vector<TransactionRecord>> history_;

    bool isLargeTransaction(double amount) const;
    bool isFrequentTransaction(int accountId) const;
    bool hasUnusualFrequency(int accountId) const;
};

} // namespace bloombank
