#pragma once

#include <bitset>
#include <cstddef>

namespace bloombank {

// Fixed-size Bloom filter over integer keys.
// Simulates HashCount independent hash functions via double hashing:
// h_i(x) = (h1(x) + i * h2(x)) mod Size.
// Space: O(Size) bits. add / possiblyContains: O(HashCount).
// False positives are possible; false negatives are not.
template <std::size_t Size, int HashCount = 3>
class BloomFilter {
public:
    void add(int key) {
        for (int i = 0; i < HashCount; ++i) {
            bits_.set(indexFor(key, i));
        }
    }

    bool possiblyContains(int key) const {
        for (int i = 0; i < HashCount; ++i) {
            if (!bits_.test(indexFor(key, i))) {
                return false;
            }
        }
        return true;
    }

private:
    std::bitset<Size> bits_;

    static std::size_t indexFor(int key, int i) {
        std::size_t h1 = static_cast<std::size_t>(static_cast<unsigned int>(key)) % Size;
        std::size_t h2 = 1 + (static_cast<std::size_t>(static_cast<unsigned int>(key)) % (Size - 1));
        return (h1 + static_cast<std::size_t>(i) * h2) % Size;
    }
};

} // namespace bloombank
