#include "bloombank/Loan.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>

namespace bloombank {

Loan::Loan(double principal, double annualInterestRatePercent, int termMonths)
    : principal_(principal), interestRate_(annualInterestRatePercent), termMonths_(termMonths) {
    if (termMonths_ <= 0) {
        throw std::invalid_argument("Loan term must be a positive number of months");
    }
}

double Loan::calculateTotalAmountDue() const {
    return principal_ * (1 + (interestRate_ / 100));
}

double Loan::calculateMonthlyPayment() const {
    return calculateTotalAmountDue() / termMonths_;
}

void Loan::displayLoanDetails() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Loan Details:" << std::endl;
    std::cout << "Principal: Rs. " << principal_ << std::endl;
    std::cout << "Interest Rate: " << interestRate_ << "%" << std::endl;
    std::cout << "Term: " << termMonths_ << " months" << std::endl;
    std::cout << "Total Amount Due: Rs. " << calculateTotalAmountDue() << std::endl;
    std::cout << "Monthly Payment: Rs. " << calculateMonthlyPayment() << std::endl;
}

} // namespace bloombank
