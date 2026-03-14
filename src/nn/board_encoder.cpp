#include "nn/board_encoder.hpp"

#include <algorithm>
#include <array>
#include <cstring>

namespace shogi {

namespace {

inline int square_to_index(int square) {
    return square; // square is already 0..80
}

inline int channel_offset(int channel, int square) {
    return channel * kBoardSquares + square_to_index(square);
}

// Assumed piece_bb layout:
// 0..13  : black pieces (P,L,N,S,G,B,R,K,+P,+L,+N,+S,+B,+R)
// 14..27 : white pieces (same order)
inline int piece_bb_to_plane(int piece_index) {
    if (piece_index < 0 || piece_index >= kPiecePlanes) {
        return -1;
    }
    return piece_index;
}

} // namespace

std::vector<float> encode_board_planes(const Board &board) {
    std::vector<float> out(kEncoderChannels * kBoardSquares, 0.0f);
    encode_board_planes(board, out.data(), out.size());
    return out;
}

void encode_board_planes(const Board &board, float *out, std::size_t out_size) {
    if (out_size < static_cast<std::size_t>(kEncoderChannels * kBoardSquares)) {
        return;
    }
    std::memset(out, 0, sizeof(float) * kEncoderChannels * kBoardSquares);

    // 1) Piece planes
    for (int piece = 0; piece < 32; ++piece) {
        int plane = piece_bb_to_plane(piece);
        if (plane < 0) {
            continue;
        }
        const Bitboard &bb = board.piece_bb[piece];
        for (int sq = 0; sq < kBoardSquares; ++sq) {
            if (bb.test(sq)) {
                out[channel_offset(plane, sq)] = 1.0f;
            }
        }
    }

    // 2) Hand planes (normalized counts)
    constexpr std::array<int, kHandPieceTypes> kHandMax = {18, 4, 4, 4, 4, 2, 2};
    for (int side = 0; side < 2; ++side) {
        for (int hp = 0; hp < kHandPieceTypes; ++hp) {
            int count = board.hand[side][hp];
            count = std::max(0, std::min(count, kHandMax[hp]));
            float value = static_cast<float>(count) / static_cast<float>(kHandMax[hp]);
            int plane = kPiecePlanes + side * kHandPieceTypes + hp;
            int base = plane * kBoardSquares;
            for (int sq = 0; sq < kBoardSquares; ++sq) {
                out[base + sq] = value;
            }
        }
    }

    // 3) Side to move plane (black=1, white=0)
    float stm = (board.side_to_move == 0) ? 1.0f : 0.0f;
    int stm_base = (kPiecePlanes + kHandPlanes) * kBoardSquares;
    for (int sq = 0; sq < kBoardSquares; ++sq) {
        out[stm_base + sq] = stm;
    }
}

} // namespace shogi
