cmake_minimum_required(VERSION 3.15)
project(BloomBank CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_library(bloombank_lib
    src/Account.cpp
    src/AccountBST.cpp
    src/TransactionHashTable.cpp
    src/FraudDetector.cpp
    src/RelationshipGraph.cpp
    src/Loan.cpp
)
target_include_directories(bloombank_lib PUBLIC include)

add_executable(bloombank src/main.cpp)
target_link_libraries(bloombank PRIVATE bloombank_lib)

enable_testing()
add_executable(bloombank_tests
    tests/test_main.cpp
    tests/test_transaction_hash_table.cpp
    tests/test_bloom_filter.cpp
    tests/test_fraud_detector.cpp
    tests/test_account_bst.cpp
    tests/test_relationship_graph.cpp
    tests/test_loan.cpp
)
target_link_libraries(bloombank_tests PRIVATE bloombank_lib)
target_include_directories(bloombank_tests PRIVATE tests)

add_test(NAME bloombank_tests COMMAND bloombank_tests)
