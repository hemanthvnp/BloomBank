#include "test_framework.hpp"
#include "bloombank/FraudDetector.hpp"

#include <string>
#include <vector>

using bloombank::FraudDetector;

TEST(SmallTransactionDoesNotFlagAccount) {
    FraudDetector detector;
    auto alerts = detector.recordTransaction(1, 100.0);
    EXPECT_TRUE(alerts.empty());
    EXPECT_FALSE(detector.possiblyFlagged(1));
}

TEST(LargeTransactionFlagsAccount) {
    FraudDetector detector;
    auto alerts = detector.recordTransaction(2, 75000.0);
    EXPECT_FALSE(alerts.empty());
    EXPECT_TRUE(detector.possiblyFlagged(2));
}

TEST(HighFrequencyTransactionsFlagAccount) {
    FraudDetector detector;
    std::vector<std::string> lastAlerts;
    for (int i = 0; i < FraudDetector::kMaxTransactionsPerMinute; ++i) {
        lastAlerts = detector.recordTransaction(3, 10.0);
    }
    EXPECT_FALSE(lastAlerts.empty());
    EXPECT_TRUE(detector.possiblyFlagged(3));
}

TEST(UnflaggedAccountReportsNoRecords) {
    FraudDetector detector;
    EXPECT_FALSE(detector.possiblyFlagged(42));
}
