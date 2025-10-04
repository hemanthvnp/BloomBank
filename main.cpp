#include <iostream>
#include <string>
using namespace std;
#include <bitset>

#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <set>
using namespace std;
const int TABLE_SIZE = 1000;
const int BLOOM_SIZE = 1000;
const int BLOOM_FILTER_SIZE=1000;
struct Transaction
{
    int transactionID;
    double amount;
    string description;
    Transaction* next;
    Transaction(int id, double amt, string desc)
    {
        transactionID = id;
        amount = amt;
        description = desc;
        next = nullptr;
    }
};
class HashTable
{
private:
    Transaction* table[TABLE_SIZE];
    int hashFunction(int key)
    {
        return key % TABLE_SIZE;
    }

public:
    HashTable()
    {
        for (int i = 0; i < TABLE_SIZE; i++)
            table[i] = nullptr;
    }

    void insertTransaction(int id, double amount, string description)
    {
        int index = hashFunction(id);
        Transaction* newTransaction = new Transaction(id, amount, description);
        if (table[index] == nullptr)
        {
            table[index] = newTransaction;
        }
        else
        {
            Transaction* temp = table[index];
            while (temp->next != nullptr)
            {
                temp = temp->next;
            }
            temp->next = newTransaction;
        }
    }
    Transaction* searchTransaction(int id)
    {
        int index = hashFunction(id);
        Transaction* temp = table[index];
        while (temp != nullptr)
        {
            if (temp->transactionID == id)
            {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }
    void displayTransactions()
    {
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            Transaction* temp = table[i];
            while (temp != nullptr)
            {
                cout << "ID: " << temp->transactionID<< ", Amount: " << temp->amount<< ", Description: " << temp->description << endl;
                temp = temp->next;
            }
        }
    }
};
class BloomFilter
{
private:
    bitset<BLOOM_FILTER_SIZE> filter;
    unordered_map<int, vector<pair<double, time_t>>> accountTransactions;
    const double LARGE_AMOUNT_THRESHOLD = 50000.0;
    const int MAX_TRANSACTIONS_PER_MIN = 5;
    const double UNUSUAL_FREQUENCY_THRESHOLD = 3.0;

    int hash1(int key) const
    {
        return key % BLOOM_FILTER_SIZE;
    }

    int hash2(int key) const
    {
        return (key / 7) % BLOOM_FILTER_SIZE;
    }

    bool isLargeTransaction(double amount) const
    {
        return amount >= LARGE_AMOUNT_THRESHOLD;
    }

    bool isFrequentTransaction(int accountId)
    {
        time_t now = time(nullptr);
        int count = 0;
        for (const auto &record : accountTransactions[accountId])
        {
            if (difftime(now, record.second) <= 60) // Check transactions in the last 60 seconds
            {
                count++;
            }
        }
        return count >= MAX_TRANSACTIONS_PER_MIN;
    }

    bool detectUnusualFrequency(int accountId)
    {
        if (accountTransactions[accountId].size() < 5)
        {
            return false;
        }

        double totalInterval = 0;
        for (size_t i = 1; i < accountTransactions[accountId].size(); ++i)
        {
            double interval = difftime(accountTransactions[accountId][i].second,
                                       accountTransactions[accountId][i - 1].second);
            totalInterval += interval;
        }
        double avgInterval = totalInterval / (accountTransactions[accountId].size() - 1);
        return avgInterval < UNUSUAL_FREQUENCY_THRESHOLD; // Flag if too frequent
    }

public:
    void add(int key)
    {
        filter.set(hash1(key));
        filter.set(hash2(key));
    }

    bool possiblyContains(int key) const
    {
        return filter.test(hash1(key)) && filter.test(hash2(key));
    }

    void recordTransaction(int accountId, double amount)
    {
        time_t now = time(nullptr);
        accountTransactions[accountId].push_back({amount, now});

        if (isLargeTransaction(amount))
        {
            cout << " ALERT: Large transaction detected for account #" << accountId << " - Amount: " << amount << endl;
        }
        if (isFrequentTransaction(accountId))
        {
            cout << " ALERT: High transaction frequency detected for account #" << accountId << endl;
        }
        if (detectUnusualFrequency(accountId))
        {
            cout << "ALERT: Unusually frequent transactions detected for account #" << accountId << endl;
        }
    }

void displaySuspiciousTransactions(int accountId) const
{
    cout << "\nSuspicious Activity Report for Account #" << accountId << endl;

    // Check if the accountId exists in the map
    if (accountTransactions.find(accountId) == accountTransactions.end())
    {
        cout << "No suspicious transactions found for this account." << endl;
        return;
    }

    bool suspiciousFound = false;
    for (const auto &record : accountTransactions.at(accountId))
    {
        double amount = record.first;
        time_t timestamp = record.second;

        if (amount >= LARGE_AMOUNT_THRESHOLD)
        {
            cout << "Large Transaction: " << amount << " at " << ctime(&timestamp);
            suspiciousFound = true;
        }
    }

    if (!suspiciousFound)
    {
        cout << "No suspicious transactions detected." << endl;
    }
}

};

struct BankAccount
{
    int accountNumber;
    string holderName;
    double balance;
    BankAccount* next;

    BankAccount(int accNum, string name, double bal)
    {
        accountNumber = accNum;
        holderName = name;
        balance = bal;
        next = nullptr;
    }
};
class Bank
{
private:
    BankAccount* head;
    BankAccount* findAccount(int accNum)
    {
        BankAccount* temp = head;
        while (temp != nullptr)
        {
            if (temp->accountNumber == accNum)
            {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }
public:
    Bank()
    {
        head = nullptr;
    }
    void createAccount(int accNum, string name, double initialBalance)
    {
        if (findAccount(accNum) != nullptr)
        {
            cout << "Account already exists!" << endl;
            return;
        }
        BankAccount* newAccount = new BankAccount(accNum, name, initialBalance);
        newAccount->next = head;
        head = newAccount;
        cout << "Account created successfully!" << endl;
    }
    void deleteAccount(int accNum)
    {
        BankAccount* temp = head;
        BankAccount* prev = nullptr;

        while (temp != nullptr && temp->accountNumber != accNum)
        {
            prev = temp;
            temp = temp->next;
        }
        if (temp == nullptr)
        {
            cout << "Account not found!" << endl;
            return;
        }
        if (prev == nullptr)
        {
            head = temp->next;
        }
        else
        {
            prev->next = temp->next;
        }
        delete temp;
        cout << "Account deleted successfully!" << endl;
    }
    void transferAmount(int senderAcc, int receiverAcc, double amount)
    {
        BankAccount* sender = findAccount(senderAcc);
        BankAccount* receiver = findAccount(receiverAcc);
        if (sender == nullptr || receiver == nullptr)
        {
            cout << "Invalid account details!" << endl;
            return;
        }
        if (sender->balance < amount)
        {
            cout << "Insufficient balance!" << endl;
            return;
        }
        sender->balance -= amount;
        receiver->balance += amount;
        cout << "Transfer successful!" << endl;
    }
    void displayAccounts()
    {
        BankAccount* temp = head;
        while (temp != nullptr)
        {
            cout << "Account No: " << temp->accountNumber
                 << ", Holder Name: " << temp->holderName
                 << ", Balance: " << temp->balance << endl;
            temp = temp->next;
        }
    }
};
class Account
{
private:
    int accountId;
    string customerName;
    double balance;
    bool isActive;
    time_t creationDate;

public:
    Account(int id, string name, double initialBalance = 0.0) : accountId(id), customerName(name), balance(initialBalance), isActive(true)
    {
        creationDate = time(nullptr);
    }
    int getId() const { return accountId; }
    string getName() const { return customerName; }
    double getBalance() const { return balance; }
    bool getIsActive() const { return isActive; }
    time_t getCreationDate() const { return creationDate; }
    void deposit(double amount)
    {
        if (amount > 0)
        {
            balance += amount;
            cout << "Deposited ₹" << amount << " to account #" << accountId << endl;
        }
    }
    bool withdraw(double amount)
    {
        if (amount > 0 && amount <= balance)
        {
            balance -= amount;
            cout << "Withdrew ₹" << amount << " from account #" << accountId << endl;
            return true;
        }
        cout << "Insufficient funds in account #" << accountId << endl;
        return false;
    }
    void deactivate()
    {
        isActive = false;
        cout << "Account #" << accountId << " has been deactivated." << endl;
    }
    void activate()
    {
        isActive = true;
        cout << "Account #" << accountId << " has been activated." << endl;
    }
    void displayInfo() const
    {
        cout << "Account #" << accountId << ":" << endl;
        cout << "  Customer: " << customerName << endl;
        cout << "  Balance: ₹" << balance << endl;
        cout << "  Status: " << (isActive ? "Active" : "Inactive") << endl;
        char dateStr[26];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", localtime(&creationDate));
        cout << "  Created on: " << dateStr << endl;
    }
};

class AccountNode
{
public:
    Account* account;
    AccountNode* left;
    AccountNode* right;
    AccountNode(Account* acc) : account(acc), left(nullptr), right(nullptr) {}
};

class AccountBST
{
private:
    AccountNode* root;
    int size;
    AccountNode* insertHelper(AccountNode* node, Account* account)
    {
        if (node == nullptr)
        {
            return new AccountNode(account);
        }
        if (account->getId() < node->account->getId())
        {
            node->left = insertHelper(node->left, account);
        }
        else if (account->getId() > node->account->getId())
        {
            node->right = insertHelper(node->right, account);
        }
        return node;
    }

    AccountNode* searchHelper(AccountNode* node, int accountId) const
    {
        if (node == nullptr || node->account->getId() == accountId)
        {
            return node;
        }
        if (accountId < node->account->getId())
        {
            return searchHelper(node->left, accountId);
        }
        else
        {
            return searchHelper(node->right, accountId);
        }
    }

    AccountNode* deleteHelper(AccountNode* node, int accountId)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        if (accountId < node->account->getId())
        {
            node->left = deleteHelper(node->left, accountId);
        }
        else if (accountId > node->account->getId())
        {
            node->right = deleteHelper(node->right, accountId);
        }
        else
        {
            if (node->left == nullptr && node->right == nullptr)
            {
                delete node;
                return nullptr;
            }
            else if (node->left == nullptr)
            {
                AccountNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr)
            {
                AccountNode* temp = node->left;
                delete node;
                return temp;
            }
            else
            {
                AccountNode* temp = node->right;
                while (temp->left != nullptr)
                {
                    temp = temp->left;
                }
                node->account = temp->account;
                node->right = deleteHelper(node->right, temp->account->getId());
            }
        }
        return node;
    }

    void inorderHelper(AccountNode* node) const
    {
        if (node == nullptr) return;
        inorderHelper(node->left);
        node->account->displayInfo();
        cout << "------------------------" << endl;
        inorderHelper(node->right);
    }

    void destroyHelper(AccountNode* node)
    {
        if (node == nullptr) return;
        destroyHelper(node->left);
        destroyHelper(node->right);
        delete node->account;
        delete node;
    }

public:
    AccountBST() : root(nullptr), size(0) {}
    ~AccountBST()
    {
        destroyHelper(root);
    }
    bool insert(Account* account)
    {
        if (search(account->getId()) != nullptr)
        {
            return false;
        }
        root = insertHelper(root, account);
        size++;
        return true;
    }
    Account* search(int accountId) const
    {
        AccountNode* result = searchHelper(root, accountId);
        return (result != nullptr) ? result->account : nullptr;
    }
    bool remove(int accountId)
    {
        if (search(accountId) == nullptr)
        {
            return false;
        }
        root = deleteHelper(root, accountId);
        size--;
        return true;
    }
    void displayAllAccounts() const
    {
        cout << "\n===== All Accounts =====" << endl;
        inorderHelper(root);
        cout << "=======================" << endl;
    }
    int getSize() const
    {
        return size;
    }
};

class RelationshipGraph
{
private:
    unordered_map<int, vector<pair<int, string>>> adjacencyList;

public:
    void addRelationship(int accountId1, int accountId2, const string& relationType)
    {
        adjacencyList[accountId1].push_back(make_pair(accountId2, relationType));
        adjacencyList[accountId2].push_back(make_pair(accountId1, relationType));
        cout << "Added " << relationType << " relationship between accounts " << accountId1 << " and " << accountId2 << endl;
    }

    bool removeRelationship(int accountId1, int accountId2)
    {
        bool removed = false;
        if (adjacencyList.find(accountId1) != adjacencyList.end())
        {
            auto& connections = adjacencyList[accountId1];
            for (auto it = connections.begin(); it != connections.end(); )
            {
                if (it->first == accountId2)
                {
                    it = connections.erase(it);
                    removed = true;
                }
                else
                {
                    ++it;
                }
            }
        }
        if (adjacencyList.find(accountId2) != adjacencyList.end())
        {
            auto& connections = adjacencyList[accountId2];
            for (auto it = connections.begin(); it != connections.end(); )
            {
                if (it->first == accountId1)
                {
                    it = connections.erase(it);
                    removed = true;
                }
                else
                {
                    ++it;
                }
            }
        }
        if (removed)
        {
            cout << "Removed relationship between accounts " << accountId1 << " and " << accountId2 << endl;
        }
        return removed;
    }

    vector<pair<int, string>> getRelationships(int accountId) const
    {
        if (adjacencyList.find(accountId) != adjacencyList.end())
        {
            return adjacencyList.at(accountId);
        }
        return {};
    }

    void displayRelationships(int accountId) const
    {
        cout << "\n===== Relationships for Account #" << accountId << " =====" << endl;
        if (adjacencyList.find(accountId) == adjacencyList.end() || adjacencyList.at(accountId).empty())
        {
            cout << "No relationships found." << endl;
            return;
        }
        for (const auto& relation : adjacencyList.at(accountId))
        {
            cout << "Connected to Account #" << relation.first << " (" << relation.second << ")" << endl;
        }
        cout << "=======================" << endl;
    }

    vector<int> findAccountsByRelationType(int accountId, const string& relationType) const
    {
        vector<int> result;
        if (adjacencyList.find(accountId) != adjacencyList.end())
        {
            for (const auto& relation : adjacencyList.at(accountId))
            {
                if (relation.second == relationType)
                {
                    result.push_back(relation.first);
                }
            }
        }
        return result;
    }

    bool areConnected(int accountId1, int accountId2) const
    {
        if (adjacencyList.find(accountId1) == adjacencyList.end())
        {
            return false;
        }
        for (const auto& relation : adjacencyList.at(accountId1))
        {
            if (relation.first == accountId2)
            {
                return true;
            }
        }
        return false;
    }

    int getTotalRelationships() const
    {
        int total = 0;
        for (const auto& entry : adjacencyList)
        {
            total += entry.second.size();
        }
        return total / 2;
    }

    void displayAllRelationships() const
    {
        cout << "\n===== All Relationships =====" << endl;
        if (adjacencyList.empty())
        {
            cout << "No relationships found." << endl;
            return;
        }
        set<pair<int, int>> processedPairs;
        for (const auto& entry : adjacencyList)
        {
            int accountId1 = entry.first;
            for (const auto& relation : entry.second)
            {
                int accountId2 = relation.first;
                string relationType = relation.second;
                pair<int, int> relationPair = make_pair(min(accountId1, accountId2), max(accountId1, accountId2));
                if (processedPairs.find(relationPair) == processedPairs.end())
                {
                    cout << "Account #" << accountId1 << " and Account #" << accountId2 << " (" << relationType << ")" << endl;
                    processedPairs.insert(relationPair);
                }
            }
        }
        cout << "=======================" << endl;
    }
};

class Loan
{
private:
    double principal;
    double interestRate;
    int termMonths;

public:
    Loan(double principal, double interestRate, int termMonths): principal(principal), interestRate(interestRate), termMonths(termMonths) {}

    double calculateTotalAmountDue() const
    {
        return principal * (1 + (interestRate / 100));
    }

    double calculateMonthlyPayment() const
    {
        double totalAmountDue = calculateTotalAmountDue();
        return totalAmountDue / termMonths;
    }

    void displayLoanDetails() const
    {
        cout << fixed << setprecision(2);
        cout << "Loan Details:" << endl;
        cout << "Principal: ₹" << principal << endl;
        cout << "Interest Rate: " << interestRate << "%" << endl;
        cout << "Term: " << termMonths << " months" << endl;
        cout << "Total Amount Due: ₹" << calculateTotalAmountDue() << endl;
        cout << "Monthly Payment: ₹" << calculateMonthlyPayment() << endl;
    }
};

int main() {
    HashTable transactionTable;
    BloomFilter fraudDetection;
    AccountBST accountBST;
    RelationshipGraph relationshipGraph;

    int choice;
    while (true) {
        cout << "\n============== Bank Management System ==============" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. Search Account" << endl;
        cout << "3. Delete Account" << endl;
        cout << "4. Deposit Money" << endl;
        cout << "5. Withdraw Money" << endl;
        cout << "6. Activate/Deactivate Account" << endl;
        cout << "7. Display All Accounts" << endl;
        cout << "8. Insert Transaction" << endl;
        cout << "9. Search Transaction" << endl;
        cout << "10. Display All Transactions" << endl;
        cout << "11. Add Relationship Between Accounts" << endl;
        cout << "12. Remove Relationship" << endl;
        cout << "13. Display Account Relationships" << endl;
        cout << "14. Find Accounts by Relation Type" << endl;
        cout << "15. Check if Accounts are Connected" << endl;
        cout << "16. Display All Relationships" << endl;
        cout << "17. Calculate Loan Details" << endl;
        cout << "18. Fraud Detection" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int accId;
                string name;
                double initialBalance;
                cout << "Enter account ID: ";
                cin >> accId;
                cout << "Enter customer name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter initial balance: ";
                cin >> initialBalance;

                Account* newAccount = new Account(accId, name, initialBalance);
                if (accountBST.insert(newAccount)) {
                    cout << "Account created successfully!" << endl;
                    fraudDetection.add(accId);
                } else {
                    cout << "Account with ID " << accId << " already exists!" << endl;
                    delete newAccount;
                }
                break;
            }
            case 2: {
                int accId;
                cout << "Enter account ID to search: ";
                cin >> accId;
                Account* account = accountBST.search(accId);
                if (account) {
                    cout << "\nAccount found:" << endl;
                    account->displayInfo();
                } else {
                    cout << "Account not found!" << endl;
                }
                break;
            }
            case 3: {
                int accId;
                cout << "Enter account ID to delete: ";
                cin >> accId;
                if (accountBST.remove(accId)) {
                    cout << "Account #" << accId << " deleted successfully!" << endl;
                } else {
                    cout << "Account not found!" << endl;
                }
                break;
            }
            case 4: {
                int accId;
                double amount;
                cout << "Enter account ID: ";
                cin >> accId;
                Account* account = accountBST.search(accId);
                if (account) {
                    cout << "Enter amount to deposit: ";
                    cin >> amount;
                    account->deposit(amount);
                    fraudDetection.recordTransaction(accId, amount);
                } else {
                    cout << "Account not found!" << endl;
                }
                break;
            }
            case 5: {
                int accId;
                double amount;
                cout << "Enter account ID: ";
                cin >> accId;
                Account* account = accountBST.search(accId);
                if (account) {
                    cout << "Enter amount to withdraw: ";
                    cin >> amount;
                    if (account->withdraw(amount)) {
                        fraudDetection.recordTransaction(accId, -amount);
                    }
                } else {
                    cout << "Account not found!" << endl;
                }
                break;
            }
            case 6: {
                int accId, status;
                cout << "Enter account ID: ";
                cin >> accId;
                Account* account = accountBST.search(accId);
                if (account) {
                    cout << "Enter 1 to activate or 0 to deactivate: ";
                    cin >> status;
                    if (status == 1) {
                        account->activate();
                    } else {
                        account->deactivate();
                    }
                } else {
                    cout << "Account not found!" << endl;
                }
                break;
            }
            case 7: {
                accountBST.displayAllAccounts();
                break;
            }
            case 8: {
                int id, accId;
                double amount;
                string description;
                cout << "Enter transaction ID: ";
                cin >> id;
                cout << "Enter account ID: ";
                cin >> accId;
                cout << "Enter amount: ";
                cin >> amount;
                cout << "Enter description: ";
                cin.ignore();
                getline(cin, description);

                transactionTable.insertTransaction(id, amount, description);
                fraudDetection.recordTransaction(accId, amount);
                cout << "Transaction inserted successfully!" << endl;
                break;
            }
            case 9: {
                int id;
                cout << "Enter transaction ID to search: ";
                cin >> id;
                Transaction* transaction = transactionTable.searchTransaction(id);
                if (transaction) {
                    cout << "Transaction found: " << endl;
                    cout << "ID: " << transaction->transactionID << endl;
                    cout << "Amount: " << transaction->amount << endl;
                    cout << "Description: " << transaction->description << endl;
                } else {
                    cout << "Transaction not found!" << endl;
                }
                break;
            }
            case 10: {
                cout << "\n===== All Transactions =====" << endl;
                transactionTable.displayTransactions();
                break;
            }
            case 11: {
                int accId1, accId2;
                string relationType;
                cout << "Enter first account ID: ";
                cin >> accId1;
                cout << "Enter second account ID: ";
                cin >> accId2;

                Account* acc1 = accountBST.search(accId1);
                Account* acc2 = accountBST.search(accId2);

                if (!acc1 || !acc2) {
                    cout << "One or both accounts not found!" << endl;
                    break;
                }

                cout << "Enter relationship type (family, friend, business, etc.): ";
                cin.ignore();
                getline(cin, relationType);
                relationshipGraph.addRelationship(accId1, accId2, relationType);
                break;
            }
            case 12: {
                int accId1, accId2;
                cout << "Enter first account ID: ";
                cin >> accId1;
                cout << "Enter second account ID: ";
                cin >> accId2;

                if (relationshipGraph.removeRelationship(accId1, accId2)) {
                    cout << "Relationship removed successfully!" << endl;
                } else {
                    cout << "Relationship not found!" << endl;
                }
                break;
            }
            case 13: {
                int accId;
                cout << "Enter account ID: ";
                cin >> accId;
                relationshipGraph.displayRelationships(accId);
                break;
            }
            case 14: {
                int accId;
                string relationType;
                cout << "Enter account ID: ";
                cin >> accId;
                cout << "Enter relationship type to search: ";
                cin.ignore();
                getline(cin, relationType);

                vector<int> relatedAccounts = relationshipGraph.findAccountsByRelationType(accId, relationType);

                if (relatedAccounts.empty()) {
                    cout << "No accounts found with relationship type '" << relationType << "'" << endl;
                } else {
                    cout << "\nAccounts related to account #" << accId << " with type '" << relationType << "':" << endl;
                    for (int id : relatedAccounts) {
                        Account* account = accountBST.search(id);
                        if (account) {
                            cout << "Account #" << id << " - " << account->getName() << endl;
                        } else {
                            cout << "Account #" << id << endl;
                        }
                    }
                }
                break;
            }
            case 15: {
                int accId1, accId2;
                cout << "Enter first account ID: ";
                cin >> accId1;
                cout << "Enter second account ID: ";
                cin >> accId2;

                if (relationshipGraph.areConnected(accId1, accId2)) {
                    cout << "Accounts #" << accId1 << " and #" << accId2 << " are connected." << endl;
                } else {
                    cout << "Accounts #" << accId1 << " and #" << accId2 << " are not connected." << endl;
                }
                break;
            }
            case 16: {
                relationshipGraph.displayAllRelationships();
                break;
            }
            case 17: {
                double principal, interestRate;
                int termMonths;
                cout << "Enter loan principal amount: ";
                cin >> principal;
                cout << "Enter annual interest rate (%): ";
                cin >> interestRate;
                cout << "Enter loan term in months: ";
                cin >> termMonths;

                Loan loan(principal, interestRate, termMonths);
                loan.displayLoanDetails();
                break;
            }
            case 18: {
                int accId;
                cout << "Enter account ID to check for suspicious activity: ";
                cin >> accId;

                if (fraudDetection.possiblyContains(accId)) {
                    fraudDetection.displaySuspiciousTransactions(accId);
                } else {
                    cout << "No transaction records found for this account." << endl;
                }
                break;
            }
            case 0: {
                cout << "Thank you for using the Bank Management System. Goodbye!" << endl;
                return 0;
            }
            default: {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
    return 0;
}
