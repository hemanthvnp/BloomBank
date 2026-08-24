#include "bloombank/AccountBST.hpp"

#include <iostream>

namespace bloombank {

using detail::AccountNode;

AccountBST::~AccountBST() {
    destroyHelper(root_);
}

AccountNode* AccountBST::insertHelper(AccountNode* node, Account* account) {
    if (node == nullptr) {
        return new AccountNode(account);
    }
    if (account->getId() < node->account->getId()) {
        node->left = insertHelper(node->left, account);
    } else if (account->getId() > node->account->getId()) {
        node->right = insertHelper(node->right, account);
    }
    return node;
}

AccountNode* AccountBST::searchHelper(AccountNode* node, int accountId) const {
    if (node == nullptr || node->account->getId() == accountId) {
        return node;
    }
    if (accountId < node->account->getId()) {
        return searchHelper(node->left, accountId);
    }
    return searchHelper(node->right, accountId);
}

AccountNode* AccountBST::detachMin(AccountNode* node, Account*& outAccount) {
    if (node->left == nullptr) {
        outAccount = node->account;
        AccountNode* right = node->right;
        delete node;
        return right;
    }
    node->left = detachMin(node->left, outAccount);
    return node;
}

AccountNode* AccountBST::deleteHelper(AccountNode* node, int accountId) {
    if (node == nullptr) {
        return nullptr;
    }
    if (accountId < node->account->getId()) {
        node->left = deleteHelper(node->left, accountId);
    } else if (accountId > node->account->getId()) {
        node->right = deleteHelper(node->right, accountId);
    } else {
        delete node->account;
        if (node->left == nullptr) {
            AccountNode* right = node->right;
            delete node;
            return right;
        }
        if (node->right == nullptr) {
            AccountNode* left = node->left;
            delete node;
            return left;
        }
        // Two children: pull the in-order successor's Account* up into this
        // node, then unlink (but do not delete-twice) the now-empty successor.
        Account* successorAccount = nullptr;
        node->right = detachMin(node->right, successorAccount);
        node->account = successorAccount;
    }
    return node;
}

void AccountBST::inorderHelper(AccountNode* node) const {
    if (node == nullptr) {
        return;
    }
    inorderHelper(node->left);
    node->account->displayInfo();
    std::cout << "------------------------" << std::endl;
    inorderHelper(node->right);
}

void AccountBST::destroyHelper(AccountNode* node) {
    if (node == nullptr) {
        return;
    }
    destroyHelper(node->left);
    destroyHelper(node->right);
    delete node->account;
    delete node;
}

bool AccountBST::insert(Account* account) {
    if (search(account->getId()) != nullptr) {
        return false;
    }
    root_ = insertHelper(root_, account);
    ++size_;
    return true;
}

Account* AccountBST::search(int accountId) const {
    AccountNode* result = searchHelper(root_, accountId);
    return result != nullptr ? result->account : nullptr;
}

bool AccountBST::remove(int accountId) {
    if (search(accountId) == nullptr) {
        return false;
    }
    root_ = deleteHelper(root_, accountId);
    --size_;
    return true;
}

void AccountBST::displayAllAccounts() const {
    std::cout << "\n===== All Accounts =====" << std::endl;
    inorderHelper(root_);
    std::cout << "=======================" << std::endl;
}

} // namespace bloombank
