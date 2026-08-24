#include "test_framework.hpp"
#include "bloombank/BloomFilter.hpp"

using bloombank::BloomFilter;

TEST(ContainsReturnsFalseForEmptyFilter) {
    BloomFilter<1000, 3> filter;
    EXPECT_FALSE(filter.possiblyContains(42));
}

TEST(ContainsReturnsTrueAfterAdd) {
    BloomFilter<1000, 3> filter;
    filter.add(42);
    EXPECT_TRUE(filter.possiblyContains(42));
}

TEST(ContainsHandlesNegativeKeys) {
    BloomFilter<1000, 3> filter;
    filter.add(-17);
    EXPECT_TRUE(filter.possiblyContains(-17));
}

TEST(DoesNotFlagUnrelatedKeyAsPresentTypically) {
    BloomFilter<1000, 3> filter;
    filter.add(1);
    filter.add(2);
    filter.add(3);
    // Not a hard guarantee (false positives are allowed by design), but with
    // only a handful of entries in a 1000-bit filter this holds in practice.
    EXPECT_FALSE(filter.possiblyContains(999999));
}
