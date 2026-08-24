#pragma once

#include "bloombank/Transaction.hpp"

#include <string>

namespace bloombank {

// Fixed-size hash table with separate chaining, keyed on transaction ID.
// insert: O(1). search: O(1) average, O(n) worst case under heavy collisions.
// Owns every Transaction node it creates and frees them on destruction.
class TransactionHashTable {
public:
    static constexpr int kTableSize = 1000;

    TransactionHashTable();
    ~TransactionHashTable();

    TransactionHashTable(const TransactionHashTable&) = delete;
    TransactionHashTable& operator=(const TransactionHashTable&) = delete;

    void insertTransaction(int id, double amount, const std::string& description);
    Transaction* searchTransaction(int id) const;
    void displayTransactions() const;

private:
    Transaction* table_[kTableSize];

    static int hashFunction(int key);
};

} // namespace bloombank
