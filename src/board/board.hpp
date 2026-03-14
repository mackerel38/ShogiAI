#pragma once

#include <string>

#include "board/bitboard.hpp"

namespace shogi {

constexpr int kBlack = 0;
constexpr int kWhite = 1;

constexpr int kPieceTypes = 14;
constexpr int kPiecePlanes = 2 * kPieceTypes;

// Piece index order (matches USI piece letters):
// P,L,N,S,G,B,R,K,+P,+L,+N,+S,+B,+R
enum PieceIndex {
    B_PAWN = 0,
    B_LANCE = 1,
    B_KNIGHT = 2,
    B_SILVER = 3,
    B_GOLD = 4,
    B_BISHOP = 5,
    B_ROOK = 6,
    B_KING = 7,
    B_PRO_PAWN = 8,
    B_PRO_LANCE = 9,
    B_PRO_KNIGHT = 10,
    B_PRO_SILVER = 11,
    B_HORSE = 12,
    B_DRAGON = 13,
    W_PAWN = 14,
    W_LANCE = 15,
    W_KNIGHT = 16,
    W_SILVER = 17,
    W_GOLD = 18,
    W_BISHOP = 19,
    W_ROOK = 20,
    W_KING = 21,
    W_PRO_PAWN = 22,
    W_PRO_LANCE = 23,
    W_PRO_KNIGHT = 24,
    W_PRO_SILVER = 25,
    W_HORSE = 26,
    W_DRAGON = 27
};

constexpr int kHandPieceTypes = 7; // P,L,N,S,G,B,R

struct Board {
    Bitboard piece_bb[32];
    int hand[2][8];
    int side_to_move;

    void clear();
    bool set_from_sfen(const std::string &sfen);
    void set_initial_position();
    int piece_at(int square) const;
};

std::string initial_sfen();

} // namespace shogi
