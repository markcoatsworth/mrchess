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
    setPositions(positions);
}

Board::~Board() {

}

void Board::draw() {
    for (unsigned int i = 0; i < _positions.size(); i++) {
        cout << _positions[i].toString() << "\t";
        if ((i + 1) % 8 == 0) {
            cout << endl;
        }
    }
    cout << endl;
}

void Board::setPositions(json &positions) {
    for (json::iterator it = positions.begin(); it != positions.end(); ++it) {
        std::string pieceClass = it.value();
        std::string piecePosition = it.key();

        PieceColor color{};
        if (pieceClass.find("black") != std::string::npos) color = PieceColor::BLACK;
        if (pieceClass.find("white") != std::string::npos) color = PieceColor::WHITE;

        PieceType type{};
        if (pieceClass.find("pawn") != std::string::npos) type = PieceType::PAWN;
        if (pieceClass.find("knight") != std::string::npos) type = PieceType::KNIGHT;
        if (pieceClass.find("bishop") != std::string::npos) type = PieceType::BISHOP;
        if (pieceClass.find("rook") != std::string::npos) type = PieceType::ROOK;
        if (pieceClass.find("queen") != std::string::npos) type = PieceType::QUEEN;
        if (pieceClass.find("king") != std::string::npos) type = PieceType::KING;
        
        Piece piece = Piece(piecePosition, color, type);
        _positions[positionIndex.at(piecePosition)] = piece;
    }
}

// TODO: Refactor this for easier reading/debugging.
// TODO: Should also think about limiting to 80 characters wide.
std::vector<std::string> Board::getPieceAvailableMoves(std::string position) {
    int index = positionIndex.at(position);
    std::vector<std::string> moves{};
    Piece piece = _positions[index];
    PieceColor opponentColor = (piece.getColor() == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;

    // Pawns need movement rules specific to each color (since they move in opposite directions)
    if (piece.getColor() == PieceColor::WHITE && piece.getType() == PieceType::PAWN) {
        if (index >= 8 && _positions[index - 8].getType() == PieceType::NONE) moves.push_back(indexPosition.at(index - 8));
        if (index >= 48 && index <= 55 && _positions[index - 16].getType() == PieceType::NONE) moves.push_back(indexPosition.at(index - 16));
        if (index >= 8 && _positions[index - 7].getColor() == opponentColor) moves.push_back(indexPosition.at(index - 7));
        if (index >= 9 && _positions[index - 9].getColor() == opponentColor) moves.push_back(indexPosition.at(index - 9));
    }
    else if (piece.getColor() == PieceColor::BLACK && piece.getType() == PieceType::PAWN) {
        if (index <= 55 && _positions[index + 8].getType() == PieceType::NONE) moves.push_back(indexPosition.at(index + 8));
        if (index >= 8 && index <= 15 && _positions[index + 16].getType() == PieceType::NONE) moves.push_back(indexPosition.at(index + 16));
        if (index <= 55 && _positions[index + 7].getColor() == opponentColor) moves.push_back(indexPosition.at(index + 7));
        if (index <= 54 && _positions[index + 9].getColor() == opponentColor) moves.push_back(indexPosition.at(index + 9));
    }
    // All other pieces have the same movement rules, regardless of color
    else if (piece.getType() == PieceType::KNIGHT) {
        if ((index - 17) >= 0 && index >= ((index / 8) * 8) + 1 && _positions[index - 17].getColor() != piece.getColor()) moves.push_back(indexPosition.at(index - 17));
        if ((index - 15) >= 0 && index < (((index / 8) + 1) * 8) - 1 && _positions[index - 15].getColor() != piece.getColor()) moves.push_back(indexPosition.at(index - 15));
        if ((index - 10) >= 0 && index >= ((index / 8) * 8) + 2 && _positions[index - 10].getColor() != piece.getColor()) moves.push_back(indexPosition.at(index - 10));
        if ((index - 6) >= 0 && index < (((index / 8) + 1) * 8) - 2 && _positions[index - 6].getColor() != piece.getColor()) moves.push_back(indexPosition.at(index - 6));
        if ((index + 6) < 64 && index >= ((index / 8) * 8) + 2 && _positions[index + 6].getColor() != piece.getColor()) moves.push_back(indexPosition.at(index + 6));
        if ((index + 10) < 64 && index < (((index / 8) + 1) * 8) - 2 && _positions[index + 10].getColor() != piece.getColor()) moves.push_back(indexPosition.at(index + 10));
        if ((index + 15) < 64 && index >= ((index / 8) * 8) + 1 && _positions[index + 15].getColor() != piece.getColor()) moves.push_back(indexPosition.at(index + 15));
        if ((index + 17) < 64 && index < (((index / 8) + 1) * 8) - 1 && _positions[index + 17].getColor() != piece.getColor()) moves.push_back(indexPosition.at(index + 17));
    }
    else if (piece.getType() == PieceType::ROOK) {
        for (int up = index - 8; up > 0; up -= 8) {
            if (_positions[up].getColor() != piece.getColor()) moves.push_back(indexPosition.at(up));
            if (_positions[up].getType() != PieceType::NONE) break;
        }
        for (int down = index + 8; down < 64; down += 8) {
            if (_positions[down].getColor() != piece.getColor()) moves.push_back(indexPosition.at(down));
            if (_positions[down].getType() != PieceType::NONE) break;
        }
        for (int left = index - 1; left >= 0 && left >= index - (index % 8); left -= 1) {
            if (_positions[left].getColor() != piece.getColor()) moves.push_back(indexPosition.at(left));
            if (_positions[left].getType() != PieceType::NONE) break;
        }
        for (int right = index + 1; right <= 63 && right <= index + (7 - (index % 8)); right += 1) {
            if (_positions[right].getColor() != piece.getColor()) moves.push_back(indexPosition.at(right));
            if (_positions[right].getColor() == opponentColor) break;
        }
    }
    else if (piece.getType() == PieceType::BISHOP) {
        for (int upleft = index - 9; upleft > 0 && (upleft % 8) != 7; upleft -= 9) {
            if (_positions[upleft].getColor() != piece.getColor()) moves.push_back(indexPosition.at(upleft));
            if (_positions[upleft].getType() != PieceType::NONE) break;
        }
        for (int upright = index - 7; upright > 0 && (upright % 8) != 0; upright -= 7) {
            if (_positions[upright].getColor() != piece.getColor()) moves.push_back(indexPosition.at(upright));
            if (_positions[upright].getType() != PieceType::NONE) break;
        }
        for (int downleft = index + 7; downleft < 64 && (downleft % 8) != 7; downleft += 7) {
            if (_positions[downleft].getColor() != piece.getColor()) moves.push_back(indexPosition.at(downleft));
            if (_positions[downleft].getType() != PieceType::NONE) break;
        }
        for (int downright = index + 9; downright < 64 && (downright % 8) != 0; downright += 9) {
            if (_positions[downright].getColor() != piece.getColor()) moves.push_back(indexPosition.at(downright));
            if (_positions[downright].getType() != PieceType::NONE) break;
        }
    }
    else if (piece.getType() == PieceType::QUEEN) {
        for (int upleft = index - 9; upleft > 0 && (upleft % 8) != 7; upleft -= 9) {
            if (_positions[upleft].getColor() != piece.getColor()) moves.push_back(indexPosition.at(upleft));
            if (_positions[upleft].getType() != PieceType::NONE) break;
        }
        for (int up = index - 8; up > 0; up -= 8) {
            if (_positions[up].getColor() != piece.getColor()) moves.push_back(indexPosition.at(up));
            if (_positions[up].getType() != PieceType::NONE) break;
        }
        for (int upright = index - 7; upright > 0 && (upright % 8) != 0; upright -= 7) {
            if (_positions[upright].getColor() != piece.getColor()) moves.push_back(indexPosition.at(upright));
            if (_positions[upright].getType() != PieceType::NONE) break;
        }
        for (int left = index - 1; left >= 0 && left >= index - (index % 8); left -= 1) {
            if (_positions[left].getColor() != piece.getColor()) moves.push_back(indexPosition.at(left));
            if (_positions[left].getType() != PieceType::NONE) break;
        }
        for (int right = index + 1; right <= 63 && right <= index + (7 - (index % 8)); right += 1) {
            if (_positions[right].getColor() != piece.getColor()) moves.push_back(indexPosition.at(right));
            if (_positions[right].getColor() == opponentColor) break;
        }
        for (int downleft = index + 7; downleft < 64 && (downleft % 8) != 7; downleft += 7) {
            if (_positions[downleft].getColor() != piece.getColor()) moves.push_back(indexPosition.at(downleft));
            if (_positions[downleft].getType() != PieceType::NONE) break;
        }
        for (int down = index + 8; down < 64; down += 8) {
            if (_positions[down].getColor() != piece.getColor()) moves.push_back(indexPosition.at(down));
            if (_positions[down].getType() != PieceType::NONE) break;
        }
        for (int downright = index + 9; downright < 64 && (downright % 8) != 0; downright += 9) {
            if (_positions[downright].getColor() != piece.getColor()) moves.push_back(indexPosition.at(downright));
            if (_positions[downright].getType() != PieceType::NONE) break;
        }
    }
    else if (piece.getType() == PieceType::KING) {
        if (index - 9 > 0 && index % 8 != 0 && _positions[index - 9].getColor() != piece.getColor()) moves.push_back(indexPosition.at(index - 9));
        if (index - 8 > 0 && _positions[index - 8].getColor() != piece.getColor()) moves.push_back(indexPosition.at(index - 8));
        if (index - 7 > 0 && index % 8 != 7 && _positions[index - 7].getColor() != piece.getColor()) moves.push_back(indexPosition.at(index - 7));
        if (index - 1 > 0 && index % 8 != 0 && _positions[index - 1].getColor() != piece.getColor()) moves.push_back(indexPosition.at(index - 1));
        if (index + 1 < 64 && index % 8 != 7 && _positions[index + 1].getColor() != piece.getColor()) moves.push_back(indexPosition.at(index + 1));
        if (index + 7 < 64 && index % 8 != 0 && _positions[index + 7].getColor() != piece.getColor()) moves.push_back(indexPosition.at(index + 7));
        if (index + 8 < 64 && _positions[index + 8].getColor() != piece.getColor()) moves.push_back(indexPosition.at(index + 8));
        if (index + 9 < 64 && index % 8 != 7 && _positions[index + 9].getColor() != piece.getColor()) moves.push_back(indexPosition.at(index + 9));
    }

    return moves;
}

json Board::getMove(PieceColor color) {
    std::unordered_map< std::string, std::vector<std::string>> allMoves{};
    json move;

    // Setup a json object of all possible moves for the specified color
    for (auto it = _positions.begin(); it != _positions.end(); it++) {
        if (it->getColor() == color) {
            std::string position = it->getPosition();
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