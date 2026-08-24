#include "test_framework.hpp"
#include "bloombank/TransactionHashTable.hpp"

using bloombank::TransactionHashTable;

TEST(InsertAndSearchFindsTransaction) {
    TransactionHashTable table;
    table.insertTransaction(1, 100.0, "groceries");
    auto* found = table.searchTransaction(1);
    EXPECT_TRUE(found != nullptr);
    EXPECT_EQ(found->amount, 100.0);
    EXPECT_EQ(found->description, "groceries");
}

TEST(SearchMissingTransactionReturnsNull) {
    TransactionHashTable table;
    EXPECT_TRUE(table.searchTransaction(999) == nullptr);
}

TEST(HandlesCollisionsWithSeparateChaining) {
    TransactionHashTable table;
    table.insertTransaction(1, 10.0, "a");
    table.insertTransaction(1 + TransactionHashTable::kTableSize, 20.0, "b");

    auto* first = table.searchTransaction(1);
    auto* second = table.searchTransaction(1 + TransactionHashTable::kTableSize);
    EXPECT_TRUE(first != nullptr);
    EXPECT_TRUE(second != nullptr);
    EXPECT_EQ(first->amount, 10.0);
    EXPECT_EQ(second->amount, 20.0);
}

TEST(HandlesNegativeTransactionIds) {
    TransactionHashTable table;
    table.insertTransaction(-5, 30.0, "refund");
    auto* found = table.searchTransaction(-5);
    EXPECT_TRUE(found != nullptr);
    EXPECT_EQ(found->amount, 30.0);
}
