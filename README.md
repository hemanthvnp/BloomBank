# BloomBank

[![CI](https://github.com/hemanthvnp/BloomBank/actions/workflows/ci.yml/badge.svg)](https://github.com/hemanthvnp/BloomBank/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)

A C++17 banking system that models accounts, transactions, relationships, and
loans on top of hand-rolled data structures, with a Bloom filter–backed fraud
detector — built as a menu-driven console app.

The point of this project is the data structures: a binary search tree for
account storage, a hash table with separate chaining for transactions, a
graph (adjacency list) for account relationships, and a Bloom filter used as
a probabilistic pre-check in front of the fraud-detection logic.

## Features

- **Account management** — create, search, delete, activate/deactivate,
  deposit, withdraw, display
- **Transactions** — insert, search, and list transaction history
- **Fraud detection** — flags large transactions, high-frequency activity in
  a rolling 60s window, and unusually short intervals between transactions;
  a Bloom filter provides a fast pre-check for "has this account ever been
  flagged" before generating a full report
- **Account relationships** — add/remove typed relationships (family,
  friend, business, ...), query by relation type, and check connectivity
  between two accounts (graph traversal)
- **Loans** — total amount due and monthly payment for a flat-interest loan

## Architecture

```
include/bloombank/     Public headers (one class/concept per file)
src/                    Implementations + the console entry point
tests/                  Self-registering unit tests (no external framework)
.github/workflows/      CI: build + run the test suite on every push
```

| Component              | Data structure                  | Key operations & complexity                          |
|-------------------------|----------------------------------|--------------------------------------------------------|
| `AccountBST`            | Binary search tree              | insert/search/remove: O(log n) avg, O(n) worst case (not self-balancing) |
| `TransactionHashTable`  | Hash table, separate chaining   | insert: O(1); search: O(1) avg, O(n) worst case         |
| `RelationshipGraph`     | Graph, adjacency list            | addRelationship/areConnected: O(1) avg, O(degree) worst |
| `BloomFilter`           | Bitset + double hashing          | add/possiblyContains: O(k), k = hash count              |
| `FraudDetector`         | Bloom filter + per-account history | recordTransaction: O(h); report: O(h) where h = that account's transaction count |
| `Loan`                  | —                                | O(1) flat-interest calculation                          |

**Design note:** classes that mutate state and report success/failure
(`AccountBST::insert`, `RelationshipGraph::removeRelationship`, ...) don't
print anything themselves — that keeps them unit-testable without capturing
stdout. Only methods explicitly named `display*` own their console output;
`src/main.cpp` is the only place user-facing menu text lives.

## Requirements

- A C++17 compiler (GCC, Clang, or MSVC)
- CMake 3.15+

## Build & run

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/bloombank        # ./build/Release/bloombank.exe on Windows/MSVC
```

Quick one-liner without CMake:

```bash
g++ -std=c++17 -Iinclude src/*.cpp -o bloombank
./bloombank
```

## Running tests

The test suite is a small self-registering harness with no external
dependencies (see `tests/test_framework.hpp`) — no package manager or network
access needed to build it.

```bash
cmake --build build --target bloombank_tests
ctest --test-dir build --output-on-failure
```

## Sample session

```
============== Bank Management System ==============
1. Create Account
...
Enter your choice: 1
Enter account ID: 101
Enter customer name: Asha Rao
Enter initial balance: 25000
Account created successfully!

Enter your choice: 5
Enter account ID: 101
Enter amount to withdraw: 60000
Insufficient funds or invalid amount.

Enter your choice: 4
Enter account ID: 101
Enter amount to deposit: 55000
Deposited Rs. 55000 to account #101
ALERT: Large transaction detected for account #101 - Amount: 55000.000000

Enter your choice: 18
Enter account ID to check for suspicious activity: 101

Suspicious Activity Report for Account #101
Large Transaction: 55000.00 at Mon Aug 24 12:03:41 2026
```

## Bugs fixed during cleanup

The original single-file prototype had a few correctness issues that are
worth calling out (and were the motivation for splitting things up):

- `AccountBST::remove` never freed the removed `Account*`, only the tree
  node wrapping it — a leak on every delete. Fixed with a `detachMin`
  helper that gives each removal exactly one clear owner.
- `TransactionHashTable` never freed its chained nodes on destruction.
- The console menu looped forever printing "Invalid choice" if `cin` ever
  hit a parse failure (e.g. typing a letter for a numeric prompt), since the
  stream's fail bit was never cleared. Input is now re-prompted safely.
- The Bloom filter was populated with *every* account ID at creation time,
  which made `possiblyContains` equivalent to "does this account exist" —
  not useful as a fraud pre-check. It's now only populated when an account
  is actually flagged.
- A second, entirely unused `Bank`/`BankAccount` linked-list implementation
  duplicated the BST-backed `Account` model and was dead code; removed.

## Possible extensions

- Self-balancing BST (AVL/red-black) to bound the O(n) worst case
- Persistence (serialize accounts/transactions to disk or a real DB)
- Smart pointers (`unique_ptr`) instead of manual `new`/`delete` for account
  ownership

## License

[MIT](LICENSE)
