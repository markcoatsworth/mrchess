#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iostream>

#include "Board.h"
#include "nlohmann/json.hpp"
#include "Piece.h"

using json = nlohmann::json;
using namespace std;

Board::Board() {

}

Board::Board(json &positions) {
    setPieces(positions);
}

Board::~Board() {

}

void Board::draw() {
    for (unsigned int i = 0; i < _pieces.size(); i++) {
        cout << _pieces[i].toString() << "\t";
        if ((i + 1) % 8 == 0) {
            cout << endl;
        }
    }
    cout << endl;
}

void Board::setPieces(json & piecePositions) {
    for (json::iterator it = piecePositions.begin(); it != piecePositions.end(); ++it) {
        std::string pieceClass = it.value();
        std::string piecePosition = it.key();

        PieceColor color{};
        if (pieceClass.find("black") != std::string::npos) {
            color = PieceColor::BLACK;
        }
        if (pieceClass.find("white") != std::string::npos) {
            color = PieceColor::WHITE;
        }

        PieceType type{};
        if (pieceClass.find("pawn") != std::string::npos) {
            type = PieceType::PAWN;
        }
        if (pieceClass.find("knight") != std::string::npos) {
            type = PieceType::KNIGHT;
        }
        if (pieceClass.find("bishop") != std::string::npos) {
            type = PieceType::BISHOP;
        }
        if (pieceClass.find("rook") != std::string::npos) {
            type = PieceType::ROOK;
        }
        if (pieceClass.find("queen") != std::string::npos) {
            type = PieceType::QUEEN;
        }
        if (pieceClass.find("king") != std::string::npos) {
            type = PieceType::KING;
        }
        
        Piece piece = Piece(color, type);
        _pieces[positionIndex.at(piecePosition)] = piece;
    }
}

std::vector<std::string> Board::getPieceAvailableMoves(std::string position) {
    int index = positionIndex.at(position);
    std::vector<std::string> moves{};
    Piece piece = _pieces[index];
    PieceColor opponentColor = (piece.getColor() == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;

    // Pawns need movement rules specific to each color (since they move in opposite directions)
    if (piece.getColor() == PieceColor::WHITE && piece.getType() == PieceType::PAWN) {
        if (index >= 8 && _pieces[index - 8].getType() == PieceType::NONE) {
            moves.push_back(indexPosition.at(index - 8));
        }
        if (index >= 48 && index <= 55 && _pieces[index - 16].getType() == PieceType::NONE) {
            moves.push_back(indexPosition.at(index - 16));
        }
        if (index >= 8 && _pieces[index - 7].getColor() == opponentColor) {
            moves.push_back(indexPosition.at(index - 7));
        }
        if (index >= 9 && _pieces[index - 9].getColor() == opponentColor) {
            moves.push_back(indexPosition.at(index - 9));
        }
    }
    else if (piece.getColor() == PieceColor::BLACK && piece.getType() == PieceType::PAWN) {
        if (index <= 55 && _pieces[index + 8].getType() == PieceType::NONE) {
            moves.push_back(indexPosition.at(index + 8));
        }
        if (index >= 8 && index <= 15 && _pieces[index + 16].getType() == PieceType::NONE) {
            moves.push_back(indexPosition.at(index + 16));
        }
        if (index <= 55 && _pieces[index + 7].getColor() == opponentColor) {
            moves.push_back(indexPosition.at(index + 7));
        }
        if (index <= 54 && _pieces[index + 9].getColor() == opponentColor) {
            moves.push_back(indexPosition.at(index + 9));
        }
    }
    // All other pieces have the same movement rules, regardless of color
    else if (piece.getType() == PieceType::KNIGHT) {
        if ((index - 17) >= 0 && index >= ((index / 8) * 8) + 1 && _pieces[index - 17].getColor() != piece.getColor()) {
            moves.push_back(indexPosition.at(index - 17));
        }
        if ((index - 15) >= 0 && index < (((index / 8) + 1) * 8) - 1 && _pieces[index - 15].getColor() != piece.getColor()) {
            moves.push_back(indexPosition.at(index - 15));
        }
        if ((index - 10) >= 0 && index >= ((index / 8) * 8) + 2 && _pieces[index - 10].getColor() != piece.getColor()) {
            moves.push_back(indexPosition.at(index - 10));
        }
        if ((index - 6) >= 0 && index < (((index / 8) + 1) * 8) - 2 && _pieces[index - 6].getColor() != piece.getColor()) {
            moves.push_back(indexPosition.at(index - 6));
        }
        if ((index + 6) < 64 && index >= ((index / 8) * 8) + 2 && _pieces[index + 6].getColor() != piece.getColor()) {
            moves.push_back(indexPosition.at(index + 6));
        }
        if ((index + 10) < 64 && index < (((index / 8) + 1) * 8) - 2 && _pieces[index + 10].getColor() != piece.getColor()) {
            moves.push_back(indexPosition.at(index + 10));
        }
        if ((index + 15) < 64 && index >= ((index / 8) * 8) + 1 && _pieces[index + 15].getColor() != piece.getColor()) {
            moves.push_back(indexPosition.at(index + 15));
        }
        if ((index + 17) < 64 && index < (((index / 8) + 1) * 8) - 1 && _pieces[index + 17].getColor() != piece.getColor()) {
            moves.push_back(indexPosition.at(index + 17));
        }
    }
    else if (piece.getType() == PieceType::ROOK) {
        for (int up = index - 8; up > 0; up -= 8) {
            if (_pieces[up].getColor() != piece.getColor()) {
                moves.push_back(indexPosition.at(up));
            }
            if (_pieces[up].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int down = index + 8; down < 64; down += 8) {
            if (_pieces[down].getColor() != piece.getColor()) {
                moves.push_back(indexPosition.at(down));
            }
            if (_pieces[down].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int left = index - 1; left >= 0 && left >= index - (index % 8); left -= 1) {
            if (_pieces[left].getColor() != piece.getColor()) {
                moves.push_back(indexPosition.at(left));
            }
            if (_pieces[left].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int right = index + 1; right <= 63 && right <= index + (7 - (index % 8)); right += 1) {
            if (_pieces[right].getColor() != piece.getColor()) {
                moves.push_back(indexPosition.at(right));
            }
            if (_pieces[right].getColor() != PieceColor::NONE) {
                break;
            }
        }
    }
    else if (piece.getType() == PieceType::BISHOP) {
        for (int upleft = index - 9; upleft > 0 && (upleft % 8) != 7; upleft -= 9) {
            if (_pieces[upleft].getColor() != piece.getColor()) {
                moves.push_back(indexPosition.at(upleft));
            }
            if (_pieces[upleft].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int upright = index - 7; upright > 0 && (upright % 8) != 0; upright -= 7) {
            if (_pieces[upright].getColor() != piece.getColor()) {
                moves.push_back(indexPosition.at(upright));
            }
            if (_pieces[upright].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int downleft = index + 7; downleft < 64 && (downleft % 8) != 7; downleft += 7) {
            if (_pieces[downleft].getColor() != piece.getColor()) {
                moves.push_back(indexPosition.at(downleft));
            }
            if (_pieces[downleft].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int downright = index + 9; downright < 64 && (downright % 8) != 0; downright += 9) {
            if (_pieces[downright].getColor() != piece.getColor()) {
                moves.push_back(indexPosition.at(downright));
            }
            if (_pieces[downright].getColor() != PieceColor::NONE) {
                break;
            }
        }
    }
    else if (piece.getType() == PieceType::QUEEN) {
        for (int upleft = index - 9; upleft > 0 && (upleft % 8) != 7; upleft -= 9) {
            if (_pieces[upleft].getColor() != piece.getColor()) {
                moves.push_back(indexPosition.at(upleft));
            }
            if (_pieces[upleft].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int up = index - 8; up > 0; up -= 8) {
            if (_pieces[up].getColor() != piece.getColor()) {
                moves.push_back(indexPosition.at(up));
            }
            if (_pieces[up].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int upright = index - 7; upright > 0 && (upright % 8) != 0; upright -= 7) {
            if (_pieces[upright].getColor() != piece.getColor()) {
                moves.push_back(indexPosition.at(upright));
            }
            if (_pieces[upright].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int left = index - 1; left >= index - (index % 8); left -= 1) {
            if (_pieces[left].getColor() != piece.getColor()) {
                moves.push_back(indexPosition.at(left));
            }
            if (_pieces[left].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int right = index + 1; right <= index + (7 - (index % 8)); right += 1) {
            if (_pieces[right].getColor() != piece.getColor()) {
                moves.push_back(indexPosition.at(right));
            }
            if (_pieces[right].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int downleft = index + 7; downleft < 64 && (downleft % 8) != 7; downleft += 7) {
            if (_pieces[downleft].getColor() != piece.getColor()) {
                moves.push_back(indexPosition.at(downleft));
            }
            if (_pieces[downleft].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int down = index + 8; down < 64; down += 8) {
            if (_pieces[down].getColor() != piece.getColor()) {
                moves.push_back(indexPosition.at(down));
            }
            if (_pieces[down].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int downright = index + 9; downright < 64 && (downright % 8) != 0; downright += 9) {
            if (_pieces[downright].getColor() != piece.getColor()) {
                moves.push_back(indexPosition.at(downright));
            }
            if (_pieces[downright].getColor() != PieceColor::NONE) {
                break;
            }
        }
    }
    else if (piece.getType() == PieceType::KING) {
        if (index - 9 > 0 && index % 8 != 0 && _pieces[index - 9].getColor() != piece.getColor()) {
            moves.push_back(indexPosition.at(index - 9));
        }
        if (index - 8 > 0 && _pieces[index - 8].getColor() != piece.getColor()) {
            moves.push_back(indexPosition.at(index - 8));
        }
        if (index - 7 > 0 && index % 8 != 7 && _pieces[index - 7].getColor() != piece.getColor()) {
            moves.push_back(indexPosition.at(index - 7));
        }
        if (index - 1 > 0 && index % 8 != 0 && _pieces[index - 1].getColor() != piece.getColor()) {
            moves.push_back(indexPosition.at(index - 1));
        }
        if (index + 1 < 64 && index % 8 != 7 && _pieces[index + 1].getColor() != piece.getColor()) {
            moves.push_back(indexPosition.at(index + 1));
        }
        if (index + 7 < 64 && index % 8 != 0 && _pieces[index + 7].getColor() != piece.getColor()) {
            moves.push_back(indexPosition.at(index + 7));
        }
        if (index + 8 < 64 && _pieces[index + 8].getColor() != piece.getColor()) {
            moves.push_back(indexPosition.at(index + 8));
        }
        if (index + 9 < 64 && index % 8 != 7 && _pieces[index + 9].getColor() != piece.getColor()) {
            moves.push_back(indexPosition.at(index + 9));
        }
        // Castling moves
        // TODO: Need to add verification that the king and rook have never moved before
        if (piece.getColor() == PieceColor::BLACK && index == 4) {
            if (_pieces[0].getColor() == PieceColor::BLACK && _pieces[0].getType() == PieceType::ROOK
                    && _pieces[1].getType() == PieceType::NONE && _pieces[2].getType() == PieceType::NONE
                    && _pieces[3].getType() == PieceType::NONE) {
                moves.push_back(indexPosition.at(2));
            }
            else if (_pieces[7].getColor() == PieceColor::BLACK && _pieces[7].getType() == PieceType::ROOK
                    && _pieces[5].getType() == PieceType::NONE && _pieces[6].getType() == PieceType::NONE) {
                moves.push_back(indexPosition.at(6));
            }
        }
        else if (piece.getColor() == PieceColor::WHITE && index == 60) {
            if (_pieces[56].getColor() == PieceColor::WHITE && _pieces[56].getType() == PieceType::ROOK
                    && _pieces[57].getType() == PieceType::NONE && _pieces[58].getType() == PieceType::NONE
                    && _pieces[59].getType() == PieceType::NONE) {
                moves.push_back(indexPosition.at(58));
            }
            else if (_pieces[63].getColor() == PieceColor::WHITE && _pieces[63].getType() == PieceType::ROOK
                    && _pieces[61].getType() == PieceType::NONE && _pieces[62].getType() == PieceType::NONE) {
                moves.push_back(indexPosition.at(62));
            }
        }
    }

    return moves;
}

json Board::getMove(PieceColor color) {
    std::unordered_map< std::string, std::vector<std::string>> allMoves{};
    json move;

    // Setup a json object of all possible moves for the specified color
    for (auto it = _pieces.begin(); it != _pieces.end(); it++) {
        if (it->getColor() == color) {
            std::string position = indexPosition.at(it - _pieces.begin());
            std::vector<std::string> moves = getPieceAvailableMoves(position);
            if (moves.size() > 0) {
                allMoves[position] = moves;
            }
        }
    }

    // Seed a random number generator using current time milliseconds
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::srand(millis);

    // Now pick a move at random
    auto it = allMoves.begin();
    std::advance(it, std::rand() % allMoves.size());
    move["from"] = it->first;
    move["to"] = it->second[std::rand() % it->second.size()];

    return move;
}