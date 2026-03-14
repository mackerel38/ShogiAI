#include "move/move.hpp"

#include <cctype>

#include "board/board.hpp"

namespace shogi {

namespace {

inline int file_from_char(char c) {
    if (c < '1' || c > '9') {
        return -1;
    }
    return c - '0';
}

inline int rank_from_char(char c) {
    if (c < 'a' || c > 'i') {
        return -1;
    }
    return (c - 'a') + 1;
}

inline int square_from_usi(char file_char, char rank_char) {
    int file = file_from_char(file_char);
    int rank = rank_from_char(rank_char);
    if (file < 1 || rank < 1) {
        return -1;
    }
    return (file - 1) * 9 + (rank - 1);
}

int drop_piece_from_char(char c, int side_to_move) {
    char u = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    int base = -1;
    switch (u) {
    case 'P': base = B_PAWN; break;
    case 'L': base = B_LANCE; break;
    case 'N': base = B_KNIGHT; break;
    case 'S': base = B_SILVER; break;
    case 'G': base = B_GOLD; break;
    case 'B': base = B_BISHOP; break;
    case 'R': base = B_ROOK; break;
    default: return -1;
    }
    return (side_to_move == kWhite) ? (base + 14) : base;
}

} // namespace

bool parse_usi_move(const std::string &usi, const Board &board, Move *out_move) {
    if (out_move == nullptr || usi.size() < 4) {
        return false;
    }

    Move mv{};
    mv.from = -1;
    mv.to = -1;
    mv.piece = -1;
    mv.promote = false;
    mv.drop = false;

    if (usi.size() >= 4 && usi[1] == '*') {
        // Drop: P*5e
        int piece = drop_piece_from_char(usi[0], board.side_to_move);
        int to = square_from_usi(usi[2], usi[3]);
        if (piece < 0 || to < 0) {
            return false;
        }
        mv.drop = true;
        mv.piece = piece;
        mv.to = to;
        *out_move = mv;
        return true;
    }

    int from = square_from_usi(usi[0], usi[1]);
    int to = square_from_usi(usi[2], usi[3]);
    if (from < 0 || to < 0) {
        return false;
    }
    mv.from = from;
    mv.to = to;
    if (usi.size() >= 5 && usi[4] == '+') {
        mv.promote = true;
    }
    mv.piece = board.piece_at(from);
    if (mv.piece < 0) {
        return false;
    }
    *out_move = mv;
    return true;
}

std::string move_to_usi(const Move &move) {
    auto file_char = [](int square) {
        int file = (square / 9) + 1;
        return static_cast<char>('0' + file);
    };
    auto rank_char = [](int square) {
        int rank = (square % 9) + 1;
        return static_cast<char>('a' + (rank - 1));
    };

    std::string out;
    if (move.drop) {
        char piece = 'P';
        int p = move.piece;
        if (p >= 14) {
            p -= 14;
        }
        switch (p) {
        case B_PAWN: piece = 'P'; break;
        case B_LANCE: piece = 'L'; break;
        case B_KNIGHT: piece = 'N'; break;
        case B_SILVER: piece = 'S'; break;
        case B_GOLD: piece = 'G'; break;
        case B_BISHOP: piece = 'B'; break;
        case B_ROOK: piece = 'R'; break;
        default: piece = 'P'; break;
        }
        out.push_back(piece);
        out.push_back('*');
        out.push_back(file_char(move.to));
        out.push_back(rank_char(move.to));
        return out;
    }

    out.push_back(file_char(move.from));
    out.push_back(rank_char(move.from));
    out.push_back(file_char(move.to));
    out.push_back(rank_char(move.to));
    if (move.promote) {
        out.push_back('+');
    }
    return out;
}

} // namespace shogi
