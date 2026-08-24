#include "bloombank/Account.hpp"
#include "bloombank/AccountBST.hpp"
#include "bloombank/FraudDetector.hpp"
#include "bloombank/Loan.hpp"
#include "bloombank/RelationshipGraph.hpp"
#include "bloombank/TransactionHashTable.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

using namespace bloombank;

namespace {

int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        }
        if (std::cin.eof()) {
            std::cout << "\nInput closed. Exiting." << std::endl;
            std::exit(0);
        }
        std::cout << "Invalid input, please enter a whole number." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

double readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        }
        if (std::cin.eof()) {
            std::cout << "\nInput closed. Exiting." << std::endl;
            std::exit(0);
        }
        std::cout << "Invalid input, please enter a number." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string value;
    std::getline(std::cin, value);
    return value;
}

void printMenu() {
    std::cout << "\n============== Bank Management System ==============" << std::endl;
    std::cout << "1. Create Account" << std::endl;
    std::cout << "2. Search Account" << std::endl;
    std::cout << "3. Delete Account" << std::endl;
    std::cout << "4. Deposit Money" << std::endl;
    std::cout << "5. Withdraw Money" << std::endl;
    std::cout << "6. Activate/Deactivate Account" << std::endl;
    std::cout << "7. Display All Accounts" << std::endl;
    std::cout << "8. Insert Transaction" << std::endl;
    std::cout << "9. Search Transaction" << std::endl;
    std::cout << "10. Display All Transactions" << std::endl;
    std::cout << "11. Add Relationship Between Accounts" << std::endl;
    std::cout << "12. Remove Relationship" << std::endl;
    std::cout << "13. Display Account Relationships" << std::endl;
    std::cout << "14. Find Accounts by Relation Type" << std::endl;
    std::cout << "15. Check if Accounts are Connected" << std::endl;
    std::cout << "16. Display All Relationships" << std::endl;
    std::cout << "17. Calculate Loan Details" << std::endl;
    std::cout << "18. Fraud Detection Report" << std::endl;
    std::cout << "0. Exit" << std::endl;
}

} // namespace

int main() {
    TransactionHashTable transactionTable;
    FraudDetector fraudDetector;
    AccountBST accountBST;
    RelationshipGraph relationshipGraph;

    while (true) {
        printMenu();
        int choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1: {
                int accId = readInt("Enter account ID: ");
                std::string name = readLine("Enter customer name: ");
                double initialBalance = readDouble("Enter initial balance: ");

                Account* newAccount = new Account(accId, name, initialBalance);
                if (accountBST.insert(newAccount)) {
                    std::cout << "Account created successfully!" << std::endl;
                } else {
                    std::cout << "Account with ID " << accId << " already exists!" << std::endl;
                    delete newAccount;
                }
                break;
            }
            case 2: {
                int accId = readInt("Enter account ID to search: ");
                Account* account = accountBST.search(accId);
                if (account) {
                    std::cout << "\nAccount found:" << std::endl;
                    account->displayInfo();
                } else {
                    std::cout << "Account not found!" << std::endl;
                }
                break;
            }
            case 3: {
                int accId = readInt("Enter account ID to delete: ");
                if (accountBST.remove(accId)) {
                    std::cout << "Account #" << accId << " deleted successfully!" << std::endl;
                } else {
                    std::cout << "Account not found!" << std::endl;
                }
                break;
            }
            case 4: {
                int accId = readInt("Enter account ID: ");
                Account* account = accountBST.search(accId);
                if (!account) {
                    std::cout << "Account not found!" << std::endl;
                    break;
                }
                double amount = readDouble("Enter amount to deposit: ");
                if (account->deposit(amount)) {
                    std::cout << "Deposited Rs. " << amount << " to account #" << accId << std::endl;
                    for (const auto& alert : fraudDetector.recordTransaction(accId, amount)) {
                        std::cout << alert << std::endl;
                    }
                } else {
                    std::cout << "Deposit amount must be positive." << std::endl;
                }
                break;
            }
            case 5: {
                int accId = readInt("Enter account ID: ");
                Account* account = accountBST.search(accId);
                if (!account) {
                    std::cout << "Account not found!" << std::endl;
                    break;
                }
                double amount = readDouble("Enter amount to withdraw: ");
                if (account->withdraw(amount)) {
                    std::cout << "Withdrew Rs. " << amount << " from account #" << accId << std::endl;
                    for (const auto& alert : fraudDetector.recordTransaction(accId, -amount)) {
                        std::cout << alert << std::endl;
                    }
                } else {
                    std::cout << "Insufficient funds or invalid amount." << std::endl;
                }
                break;
            }
            case 6: {
                int accId = readInt("Enter account ID: ");
                Account* account = accountBST.search(accId);
                if (!account) {
                    std::cout << "Account not found!" << std::endl;
                    break;
                }
                int status = readInt("Enter 1 to activate or 0 to deactivate: ");
                if (status == 1) {
                    account->activate();
                    std::cout << "Account #" << accId << " has been activated." << std::endl;
                } else {
                    account->deactivate();
                    std::cout << "Account #" << accId << " has been deactivated." << std::endl;
                }
                break;
            }
            case 7: {
                accountBST.displayAllAccounts();
                break;
            }
            case 8: {
                int id = readInt("Enter transaction ID: ");
                int accId = readInt("Enter account ID: ");
                double amount = readDouble("Enter amount: ");
                std::string description = readLine("Enter description: ");

                transactionTable.insertTransaction(id, amount, description);
                std::cout << "Transaction inserted successfully!" << std::endl;
                for (const auto& alert : fraudDetector.recordTransaction(accId, amount)) {
                    std::cout << alert << std::endl;
                }
                break;
            }
            case 9: {
                int id = readInt("Enter transaction ID to search: ");
                Transaction* transaction = transactionTable.searchTransaction(id);
                if (transaction) {
                    std::cout << "Transaction found:" << std::endl;
                    std::cout << "ID: " << transaction->transactionId << std::endl;
                    std::cout << "Amount: " << transaction->amount << std::endl;
                    std::cout << "Description: " << transaction->description << std::endl;
                } else {
                    std::cout << "Transaction not found!" << std::endl;
                }
                break;
            }
            case 10: {
                std::cout << "\n===== All Transactions =====" << std::endl;
                transactionTable.displayTransactions();
                break;
            }
            case 11: {
                int accId1 = readInt("Enter first account ID: ");
                int accId2 = readInt("Enter second account ID: ");

                if (!accountBST.search(accId1) || !accountBST.search(accId2)) {
                    std::cout << "One or both accounts not found!" << std::endl;
                    break;
                }

                std::string relationType = readLine("Enter relationship type (family, friend, business, etc.): ");
                relationshipGraph.addRelationship(accId1, accId2, relationType);
                std::cout << "Added " << relationType << " relationship between accounts "
                           << accId1 << " and " << accId2 << std::endl;
                break;
            }
            case 12: {
                int accId1 = readInt("Enter first account ID: ");
                int accId2 = readInt("Enter second account ID: ");

                if (relationshipGraph.removeRelationship(accId1, accId2)) {
                    std::cout << "Relationship removed successfully!" << std::endl;
                } else {
                    std::cout << "Relationship not found!" << std::endl;
                }
                break;
            }
            case 13: {
                int accId = readInt("Enter account ID: ");
                relationshipGraph.displayRelationships(accId);
                break;
            }
            case 14: {
                int accId = readInt("Enter account ID: ");
                std::string relationType = readLine("Enter relationship type to search: ");

                std::vector<int> relatedAccounts = relationshipGraph.findAccountsByRelationType(accId, relationType);

                if (relatedAccounts.empty()) {
                    std::cout << "No accounts found with relationship type '" << relationType << "'" << std::endl;
                } else {
                    std::cout << "\nAccounts related to account #" << accId << " with type '" << relationType << "':" << std::endl;
                    for (int id : relatedAccounts) {
                        Account* account = accountBST.search(id);
                        if (account) {
                            std::cout << "Account #" << id << " - " << account->getName() << std::endl;
                        } else {
                            std::cout << "Account #" << id << std::endl;
                        }
                    }
                }
                break;
            }
            case 15: {
                int accId1 = readInt("Enter first account ID: ");
                int accId2 = readInt("Enter second account ID: ");

                if (relationshipGraph.areConnected(accId1, accId2)) {
                    std::cout << "Accounts #" << accId1 << " and #" << accId2 << " are connected." << std::endl;
                } else {
                    std::cout << "Accounts #" << accId1 << " and #" << accId2 << " are not connected." << std::endl;
                }
                break;
            }
            case 16: {
                relationshipGraph.displayAllRelationships();
                break;
            }
            case 17: {
                double principal = readDouble("Enter loan principal amount: ");
                double interestRate = readDouble("Enter annual interest rate (%): ");
                int termMonths = readInt("Enter loan term in months: ");
                try {
                    Loan loan(principal, interestRate, termMonths);
                    loan.displayLoanDetails();
                } catch (const std::invalid_argument& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            }
            case 18: {
                int accId = readInt("Enter account ID to check for suspicious activity: ");
                if (fraudDetector.possiblyFlagged(accId)) {
                    fraudDetector.displaySuspiciousTransactions(accId);
                } else {
                    std::cout << "No suspicious activity recorded for this account." << std::endl;
                }
                break;
            }
            case 0: {
                std::cout << "Thank you for using the Bank Management System. Goodbye!" << std::endl;
                return 0;
            }
            default: {
                std::cout << "Invalid choice. Please try again." << std::endl;
            }
        }
    }
}
