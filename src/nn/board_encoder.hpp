#pragma once

#include <cstddef>
#include <vector>

#include "board/board.hpp"

namespace shogi {

constexpr int kBoardFiles = 9;
constexpr int kBoardRanks = 9;
constexpr int kBoardSquares = kBoardFiles * kBoardRanks;
constexpr int kHandPlanes = 2 * kHandPieceTypes;
constexpr int kSidePlane = 1;
constexpr int kEncoderChannels = kPiecePlanes + kHandPlanes + kSidePlane; // 43

std::vector<float> encode_board_planes(const Board &board);
void encode_board_planes(const Board &board, float *out, std::size_t out_size);

} // namespace shogi
