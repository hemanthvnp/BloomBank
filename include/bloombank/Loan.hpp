#pragma once

namespace bloombank {

// Simple flat-interest loan: total due = principal * (1 + rate / 100),
// split evenly across the term.
class Loan {
public:
    // Throws std::invalid_argument if termMonths <= 0.
    Loan(double principal, double annualInterestRatePercent, int termMonths);

    double calculateTotalAmountDue() const;
    double calculateMonthlyPayment() const;
    void displayLoanDetails() const;

private:
    double principal_;
    double interestRate_;
    int termMonths_;
};

} // namespace bloombank
