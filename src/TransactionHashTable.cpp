#include "bloombank/TransactionHashTable.hpp"

#include <iostream>

namespace bloombank {

TransactionHashTable::TransactionHashTable() {
    for (int i = 0; i < kTableSize; ++i) {
        table_[i] = nullptr;
    }
}

TransactionHashTable::~TransactionHashTable() {
    for (int i = 0; i < kTableSize; ++i) {
        Transaction* current = table_[i];
        while (current != nullptr) {
            Transaction* next = current->next;
            delete current;
            current = next;
        }
    }
}

int TransactionHashTable::hashFunction(int key) {
    // Cast to unsigned before the modulo so negative IDs still map into
    // [0, kTableSize) instead of producing a negative index.
    return static_cast<int>(static_cast<unsigned int>(key) % static_cast<unsigned int>(kTableSize));
}

void TransactionHashTable::insertTransaction(int id, double amount, const std::string& description) {
    int index = hashFunction(id);
    Transaction* newTransaction = new Transaction(id, amount, description);
    newTransaction->next = table_[index];
    table_[index] = newTransaction;
}

Transaction* TransactionHashTable::searchTransaction(int id) const {
    Transaction* current = table_[hashFunction(id)];
    while (current != nullptr) {
        if (current->transactionId == id) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void TransactionHashTable::displayTransactions() const {
    for (int i = 0; i < kTableSize; ++i) {
        for (Transaction* current = table_[i]; current != nullptr; current = current->next) {
            std::cout << "ID: " << current->transactionId
                       << ", Amount: " << current->amount
                       << ", Description: " << current->description << std::endl;
        }
    }
}

} // namespace bloombank
