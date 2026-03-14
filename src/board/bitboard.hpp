#pragma once

#include <cstdint>

namespace shogi {

struct Bitboard {
    uint64_t lo;
    uint64_t hi;

    constexpr Bitboard() : lo(0), hi(0) {}
    constexpr Bitboard(uint64_t lo_bits, uint64_t hi_bits) : lo(lo_bits), hi(hi_bits) {}

    inline bool test(int square) const {
        if (square < 64) {
            return (lo >> square) & 1ULL;
        }
        return (hi >> (square - 64)) & 1ULL;
    }

    inline void set(int square) {
        if (square < 64) {
            lo |= (1ULL << square);
            return;
        }
        hi |= (1ULL << (square - 64));
    }

    inline void clear(int square) {
        if (square < 64) {
            lo &= ~(1ULL << square);
            return;
        }
        hi &= ~(1ULL << (square - 64));
    }
};

} // namespace shogi
