#pragma once

// Minimal, dependency-free unit test harness. Tests self-register via the
// TEST() macro; test_main.cpp just calls runAllTests().

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace testing {

struct TestCase {
    std::string name;
    std::function<void()> fn;
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> instance;
    return instance;
}

struct Registrar {
    Registrar(std::string name, std::function<void()> fn) {
        registry().push_back({std::move(name), std::move(fn)});
    }
};

inline int& failureCount() {
    static int count = 0;
    return count;
}

inline int& assertionCount() {
    static int count = 0;
    return count;
}

} // namespace testing

#define TEST(name)                                                              \
    static void name();                                                        \
    static testing::Registrar registrar_##name(#name, name);                   \
    static void name()

#define EXPECT_TRUE(cond)                                                       \
    do {                                                                        \
        ++testing::assertionCount();                                           \
        if (!(cond)) {                                                          \
            ++testing::failureCount();                                         \
            std::cerr << "  FAILED: " << #cond << " at " << __FILE__ << ":"    \
                       << __LINE__ << std::endl;                                \
        }                                                                       \
    } while (0)

#define EXPECT_FALSE(cond) EXPECT_TRUE(!(cond))
#define EXPECT_EQ(actual, expected) EXPECT_TRUE((actual) == (expected))

// For floating-point comparisons, where exact equality is not reliable.
#define EXPECT_NEAR(actual, expected, epsilon) \
    EXPECT_TRUE(((actual) > (expected) - (epsilon)) && ((actual) < (expected) + (epsilon)))

inline int runAllTests() {
    for (const auto& test : testing::registry()) {
        std::cout << "[RUN] " << test.name << std::endl;
        test.fn();
    }
    std::cout << "\n" << testing::assertionCount() << " assertions, "
               << testing::failureCount() << " failed." << std::endl;
    return testing::failureCount() == 0 ? 0 : 1;
}
