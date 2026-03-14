#pragma once

#include <string>

namespace shogi {

struct Move {
    int from;
    int to;
    int piece;
    bool promote;
    bool drop;
};

class Board;

bool parse_usi_move(const std::string &usi, const Board &board, Move *out_move);
std::string move_to_usi(const Move &move);

} // namespace shogi
