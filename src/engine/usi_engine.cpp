#include "engine/usi_engine.hpp"

#include <iostream>
#include <sstream>
#include <string>

namespace shogi {

void UsiEngine::run() {
    board_.set_initial_position();

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "usi") {
            std::cout << "id name ShogiAI" << std::endl;
            std::cout << "id author Codex" << std::endl;
            std::cout << "usiok" << std::endl;
            continue;
        }
        if (line == "isready") {
            std::cout << "readyok" << std::endl;
            continue;
        }
        if (line.rfind("setoption", 0) == 0) {
            continue;
        }
        if (line == "usinewgame") {
            board_.set_initial_position();
            continue;
        }
        if (line.rfind("position", 0) == 0) {
            handle_position(line);
            continue;
        }
        if (line.rfind("go", 0) == 0) {
            handle_go();
            continue;
        }
        if (line == "stop") {
            // No async search yet; reply with resign.
            std::cout << "bestmove resign" << std::endl;
            continue;
        }
        if (line == "quit") {
            break;
        }
    }
}

void UsiEngine::handle_position(const std::string &line) {
    std::istringstream iss(line);
    std::string token;
    iss >> token; // position
    iss >> token;
    if (token == "startpos") {
        board_.set_initial_position();
    } else if (token == "sfen") {
        std::string sfen_board;
        std::string sfen_side;
        std::string sfen_hand;
        std::string sfen_ply;
        iss >> sfen_board >> sfen_side >> sfen_hand;
        if (!(iss >> sfen_ply)) {
            sfen_ply = "1";
        }
        std::string sfen = sfen_board + " " + sfen_side + " " + sfen_hand + " " + sfen_ply;
        board_.set_from_sfen(sfen);
    }

    // Optional moves list (ignored for now)
    bool in_moves = false;
    while (iss >> token) {
        if (token == "moves") {
            in_moves = true;
            continue;
        }
        if (in_moves) {
            apply_usi_move(token);
        }
    }
}

void UsiEngine::handle_go() {
    // TODO: implement search/move generation.
    std::cout << "bestmove resign" << std::endl;
}

void UsiEngine::apply_usi_move(const std::string &usi) {
    Move mv{};
    if (!parse_usi_move(usi, board_, &mv)) {
        return;
    }

    if (mv.drop) {
        int hand_index = -1;
        int base = mv.piece;
        if (base >= 14) {
            base -= 14;
        }
        switch (base) {
        case B_PAWN: hand_index = 0; break;
        case B_LANCE: hand_index = 1; break;
        case B_KNIGHT: hand_index = 2; break;
        case B_SILVER: hand_index = 3; break;
        case B_GOLD: hand_index = 4; break;
        case B_BISHOP: hand_index = 5; break;
        case B_ROOK: hand_index = 6; break;
        default: break;
        }
        if (hand_index >= 0 && board_.hand[board_.side_to_move][hand_index] > 0) {
            board_.hand[board_.side_to_move][hand_index] -= 1;
            board_.piece_bb[mv.piece].set(mv.to);
        }
    } else {
        int piece = board_.piece_at(mv.from);
        if (piece < 0) {
            return;
        }
        // Capture
        int captured = board_.piece_at(mv.to);
        if (captured >= 0) {
            board_.piece_bb[captured].clear(mv.to);
            int base = captured >= 14 ? (captured - 14) : captured;
            // Demote captured piece for hand
            switch (base) {
            case B_PRO_PAWN: base = B_PAWN; break;
            case B_PRO_LANCE: base = B_LANCE; break;
            case B_PRO_KNIGHT: base = B_KNIGHT; break;
            case B_PRO_SILVER: base = B_SILVER; break;
            case B_HORSE: base = B_BISHOP; break;
            case B_DRAGON: base = B_ROOK; break;
            default: break;
            }
            int hand_index = -1;
            switch (base) {
            case B_PAWN: hand_index = 0; break;
            case B_LANCE: hand_index = 1; break;
            case B_KNIGHT: hand_index = 2; break;
            case B_SILVER: hand_index = 3; break;
            case B_GOLD: hand_index = 4; break;
            case B_BISHOP: hand_index = 5; break;
            case B_ROOK: hand_index = 6; break;
            default: break;
            }
            if (hand_index >= 0) {
                board_.hand[board_.side_to_move][hand_index] += 1;
            }
        }

        // Move piece
        board_.piece_bb[piece].clear(mv.from);

        int final_piece = piece;
        if (mv.promote) {
            int base = piece >= 14 ? (piece - 14) : piece;
            int promoted = -1;
            switch (base) {
            case B_PAWN: promoted = B_PRO_PAWN; break;
            case B_LANCE: promoted = B_PRO_LANCE; break;
            case B_KNIGHT: promoted = B_PRO_KNIGHT; break;
            case B_SILVER: promoted = B_PRO_SILVER; break;
            case B_BISHOP: promoted = B_HORSE; break;
            case B_ROOK: promoted = B_DRAGON; break;
            default: promoted = -1; break;
            }
            if (promoted >= 0) {
                final_piece = (piece >= 14) ? (promoted + 14) : promoted;
            }
        }
        board_.piece_bb[final_piece].set(mv.to);
    }

    board_.side_to_move = (board_.side_to_move == kBlack) ? kWhite : kBlack;
}

} // namespace shogi
