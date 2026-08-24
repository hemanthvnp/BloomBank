#pragma once

#include <string>

namespace bloombank {

// Intrusive singly-linked node used by TransactionHashTable's chaining buckets.
struct Transaction {
    int transactionId;
    double amount;
    std::string description;
    Transaction* next;

    Transaction(int id, double amt, std::string desc)
        : transactionId(id), amount(amt), description(std::move(desc)), next(nullptr) {}
};

} // namespace bloombank
