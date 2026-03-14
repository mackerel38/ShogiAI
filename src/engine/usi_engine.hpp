#pragma once

#include "board/board.hpp"
#include "move/move.hpp"

namespace shogi {

class UsiEngine {
public:
    void run();

private:
    Board board_;

    void handle_position(const std::string &line);
    void handle_go();
    void apply_usi_move(const std::string &usi);
};

} // namespace shogi
