#include "test_framework.hpp"
#include "bloombank/Loan.hpp"

#include <stdexcept>

using bloombank::Loan;

TEST(CalculatesTotalAmountDueWithInterest) {
    Loan loan(100000.0, 10.0, 12);
    EXPECT_NEAR(loan.calculateTotalAmountDue(), 110000.0, 0.01);
}

TEST(CalculatesMonthlyPayment) {
    Loan loan(120000.0, 0.0, 12);
    EXPECT_NEAR(loan.calculateMonthlyPayment(), 10000.0, 0.01);
}

TEST(ZeroOrNegativeTermThrows) {
    bool threw = false;
    try {
        Loan loan(1000.0, 5.0, 0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    EXPECT_TRUE(threw);
}
