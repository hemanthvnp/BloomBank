#include "test_framework.hpp"
#include "bloombank/Account.hpp"
#include "bloombank/AccountBST.hpp"

#include <string>

using bloombank::Account;
using bloombank::AccountBST;

TEST(InsertAndSearchFindsAccount) {
    AccountBST bst;
    bst.insert(new Account(1, "Alice", 100.0));
    Account* found = bst.search(1);
    EXPECT_TRUE(found != nullptr);
    EXPECT_EQ(found->getName(), "Alice");
    EXPECT_EQ(bst.size(), 1);
}

TEST(InsertRejectsDuplicateIdAndKeepsOwnershipWithCaller) {
    AccountBST bst;
    EXPECT_TRUE(bst.insert(new Account(1, "Alice", 100.0)));

    Account* duplicate = new Account(1, "Bob", 50.0);
    EXPECT_FALSE(bst.insert(duplicate));
    delete duplicate; // rejected accounts are not owned by the tree
    EXPECT_EQ(bst.size(), 1);
}

TEST(RemoveLeafNode) {
    AccountBST bst;
    bst.insert(new Account(5, "A", 0));
    bst.insert(new Account(3, "B", 0));
    bst.insert(new Account(7, "C", 0));

    EXPECT_TRUE(bst.remove(3));
    EXPECT_TRUE(bst.search(3) == nullptr);
    EXPECT_EQ(bst.size(), 2);
}

TEST(RemoveNodeWithTwoChildrenPreservesOtherAccounts) {
    AccountBST bst;
    int ids[] = {5, 3, 7, 2, 4, 6, 8};
    for (int id : ids) {
        bst.insert(new Account(id, "N" + std::to_string(id), 0));
    }

    EXPECT_TRUE(bst.remove(5));
    EXPECT_TRUE(bst.search(5) == nullptr);

    int remaining[] = {3, 7, 2, 4, 6, 8};
    for (int id : remaining) {
        EXPECT_TRUE(bst.search(id) != nullptr);
    }
    EXPECT_EQ(bst.size(), 6);
}

TEST(RemoveMissingIdReturnsFalse) {
    AccountBST bst;
    EXPECT_FALSE(bst.remove(123));
}
