#include "board/board.hpp"

#include <array>
#include <cctype>
#include <cstring>

namespace shogi {

namespace {

constexpr int kBoardSquares = 81;

int piece_from_usi(char c, bool promoted) {
    bool white = std::islower(static_cast<unsigned char>(c)) != 0;
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
    case 'K': base = B_KING; break;
    default: return -1;
    }

    if (promoted) {
        switch (base) {
        case B_PAWN: base = B_PRO_PAWN; break;
        case B_LANCE: base = B_PRO_LANCE; break;
        case B_KNIGHT: base = B_PRO_KNIGHT; break;
        case B_SILVER: base = B_PRO_SILVER; break;
        case B_BISHOP: base = B_HORSE; break;
        case B_ROOK: base = B_DRAGON; break;
        default: return -1; // K,G cannot promote
        }
    }

    return white ? (base + 14) : base;
}

int hand_index_from_usi(char c) {
    char u = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    switch (u) {
    case 'P': return 0;
    case 'L': return 1;
    case 'N': return 2;
    case 'S': return 3;
    case 'G': return 4;
    case 'B': return 5;
    case 'R': return 6;
    default: return -1;
    }
}

} // namespace

void Board::clear() {
    for (int i = 0; i < 32; ++i) {
        piece_bb[i] = Bitboard();
    }
    std::memset(hand, 0, sizeof(hand));
    side_to_move = kBlack;
}

bool Board::set_from_sfen(const std::string &sfen) {
    clear();

    // Split SFEN: board / side / hand / ply
    std::array<std::string, 4> parts;
    int part_index = 0;
    std::string current;
    for (char ch : sfen) {
        if (ch == ' ') {
            if (part_index >= 4) {
                return false;
            }
            parts[part_index++] = current;
            current.clear();
        } else {
            current.push_back(ch);
        }
    }
    if (part_index < 4) {
        parts[part_index++] = current;
    }
    if (part_index < 3) {
        return false;
    }

    const std::string &board_part = parts[0];
    const std::string &side_part = parts[1];
    const std::string &hand_part = parts[2];

    int rank = 0;
    int file = 0;
    bool promoted = false;

    for (std::size_t i = 0; i < board_part.size(); ++i) {
        char c = board_part[i];
        if (c == '/') {
            if (file != 9) {
                return false;
            }
            rank++;
            file = 0;
            continue;
        }
        if (c == '+') {
            promoted = true;
            continue;
        }
        if (std::isdigit(static_cast<unsigned char>(c)) != 0) {
            int empty = c - '0';
            file += empty;
            promoted = false;
            continue;
        }

        int piece = piece_from_usi(c, promoted);
        promoted = false;
        if (piece < 0) {
            return false;
        }
        if (rank >= 9 || file >= 9) {
            return false;
        }
        // SFEN lists files 9..1 (left->right). Internal file index is (file-1),
        // so map leftmost to file 9 => index 8.
        int file_index = 8 - file;
        int square = file_index * 9 + rank;
        piece_bb[piece].set(square);
        file++;
    }

    if (rank != 8 || file != 9) {
        return false;
    }

    if (side_part == "b") {
        side_to_move = kBlack;
    } else if (side_part == "w") {
        side_to_move = kWhite;
    } else {
        return false;
    }

    if (hand_part != "-") {
        int count = 0;
        for (std::size_t i = 0; i < hand_part.size(); ++i) {
            char c = hand_part[i];
            if (std::isdigit(static_cast<unsigned char>(c)) != 0) {
                count = count * 10 + (c - '0');
                continue;
            }
            if (count == 0) {
                count = 1;
            }
            int idx = hand_index_from_usi(c);
            if (idx < 0) {
                return false;
            }
            bool white = std::islower(static_cast<unsigned char>(c)) != 0;
            int side = white ? kWhite : kBlack;
            hand[side][idx] += count;
            count = 0;
        }
    }

    return true;
}

void Board::set_initial_position() {
    set_from_sfen(initial_sfen());
}

std::string initial_sfen() {
    return "lnsgkgsnl/1r5b1/p1ppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1";
}

int Board::piece_at(int square) const {
    for (int i = 0; i < 32; ++i) {
        if (piece_bb[i].test(square)) {
            return i;
        }
    }
    return -1;
}

} // namespace shogi
