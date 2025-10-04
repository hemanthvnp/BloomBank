# BloomBank: A Data-Driven Banking System with Fraud Detection
Developed a C++-based banking system using Hash Tables, BSTs, and Graphs for efficient data management, integrating a Bloom Filter to detect fraudulent transactions and a Loan module for EMI computation in a menu-driven console interface.

## Features

- **Account Management**
  - Create, search, delete, activate/deactivate accounts
  - Deposit and withdraw money
  - Display account details

- **Transaction Management**
  - Insert, search, and display transactions
  - Track transaction history per account

- **Fraud Detection**
  - Detect unusually large transactions
  - Monitor high-frequency transactions
  - Bloom Filter implementation to flag suspicious accounts

- **Relationships Between Accounts**
  - Add, remove, and display relationships (family, friend, business, etc.)
  - Query accounts by relation type
  - Check connectivity between accounts

- **Loan Management**
  - Calculate total amount due
  - Compute monthly payments
  - Display detailed loan information

- **Data Structures Used**
  - Hash Table for transactions
  - Bloom Filter for quick fraud detection
  - BST (Binary Search Tree) for accounts
  - Graph for relationships between accounts

## Requirements

- **C++ Compiler** (supports C++11 or later)
- Standard C++ libraries (`<iostream>`, `<vector>`, `<unordered_map>`, `<bitset>`, `<set>`, `<ctime>`)

## How to Compile and Run

1. Save your code as `bank_system.cpp`.
2. Open a terminal or command prompt in the folder containing the file.
3. Compile the program using:

```bash
g++ -std=c++11 -o bank_system bank_system.cpp
