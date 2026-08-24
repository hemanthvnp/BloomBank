#pragma once

#include "bloombank/Account.hpp"

namespace bloombank {

namespace detail {
struct AccountNode {
    Account* account;
    AccountNode* left;
    AccountNode* right;
    explicit AccountNode(Account* acc) : account(acc), left(nullptr), right(nullptr) {}
};
} // namespace detail

// Unbalanced binary search tree keyed on account ID.
// Takes ownership of every Account* passed to insert(): remove() and the
// destructor free them. average O(log n) insert/search/remove; O(n) worst
// case on adversarial (e.g. sorted) insertion order since this tree does
// not self-balance.
class AccountBST {
public:
    AccountBST() = default;
    ~AccountBST();

    AccountBST(const AccountBST&) = delete;
    AccountBST& operator=(const AccountBST&) = delete;

    // Returns false without taking ownership if the ID already exists.
    bool insert(Account* account);
    Account* search(int accountId) const;
    // Returns false if the ID does not exist. On success, the Account is deleted.
    bool remove(int accountId);
    void displayAllAccounts() const;
    int size() const { return size_; }

private:
    detail::AccountNode* root_ = nullptr;
    int size_ = 0;

    detail::AccountNode* insertHelper(detail::AccountNode* node, Account* account);
    detail::AccountNode* searchHelper(detail::AccountNode* node, int accountId) const;
    detail::AccountNode* deleteHelper(detail::AccountNode* node, int accountId);
    // Unlinks the leftmost node of the subtree rooted at `node`, frees the
    // node itself, and hands its Account* back via outAccount without deleting it.
    detail::AccountNode* detachMin(detail::AccountNode* node, Account*& outAccount);
    void inorderHelper(detail::AccountNode* node) const;
    void destroyHelper(detail::AccountNode* node);
};

} // namespace bloombank
