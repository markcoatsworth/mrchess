#include <chrono>
#include <fstream>
#include <iostream>
#include <iostream>
#include <thread>
#include <typeinfo>

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
        
        Piece piece = Piece(color, type);
        _positions[positionIndex.at(piecePosition)] = piece;
    }
}

std::vector<std::string> Board::getPieceAvailableMoves(std::string position) {
    int index = positionIndex.at(position);
    std::vector<std::string> moves{};
    Piece piece = _positions[index];

    if (piece.getColor() == PieceColor::BLACK) {
        if (piece.getType() == PieceType::PAWN) {

        }
    }
    else if (piece.getColor() == PieceColor::WHITE) {
        if (piece.getType() == PieceType::PAWN) {
            if (index >= 8 && _positions[index - 8].getType() == PieceType::NONE) moves.push_back(indexPosition.at(index - 8));
            if (index >= 48 && index <= 55 && _positions[index - 16].getType() == PieceType::NONE) moves.push_back(indexPosition.at(index - 16));
            if (index >= 8 && _positions[index - 7].getColor() == PieceColor::BLACK) moves.push_back(indexPosition.at(index - 7));
            if (index >= 9 && _positions[index - 9].getColor() == PieceColor::BLACK) moves.push_back(indexPosition.at(index - 9));
        }
        else if (piece.getType() == PieceType::KNIGHT) {
            if ((index - 17) >= 0 && index >= ((index / 8) * 8) + 1 && _positions[index - 17].getColor() != PieceColor::WHITE) moves.push_back(indexPosition.at(index - 17));
            if ((index - 15) >= 0 && index < (((index / 8) + 1) * 8) - 1 && _positions[index - 15].getColor() != PieceColor::WHITE) moves.push_back(indexPosition.at(index - 15));
            if ((index - 10) >= 0 && index >= ((index / 8) * 8) + 2 && _positions[index - 10].getColor() != PieceColor::WHITE) moves.push_back(indexPosition.at(index - 10));
            if ((index - 6) >= 0 && index < (((index / 8) + 1) * 8) - 2 && _positions[index - 6].getColor() != PieceColor::WHITE) moves.push_back(indexPosition.at(index - 6));
            if ((index + 6) < 64 && index >= ((index / 8) * 8) + 2 && _positions[index + 6].getColor() != PieceColor::WHITE) moves.push_back(indexPosition.at(index + 6));
            if ((index + 10) < 64 && index < (((index / 8) + 1) * 8) - 2 && _positions[index + 10].getColor() != PieceColor::WHITE) moves.push_back(indexPosition.at(index + 10));
            if ((index + 15) < 64 && index >= ((index / 8) * 8) + 1 && _positions[index + 15].getColor() != PieceColor::WHITE) moves.push_back(indexPosition.at(index + 15));
            if ((index + 17) < 64 && index < (((index / 8) + 1) * 8) - 1 && _positions[index + 17].getColor() != PieceColor::WHITE) moves.push_back(indexPosition.at(index + 17));
        }
        else if (piece.getType() == PieceType::ROOK) {
            for (int up = index - 8; up > 0; up -= 8) {
                if (_positions[up].getColor() != PieceColor::WHITE) moves.push_back(indexPosition.at(up));
                if (_positions[up].getType() != PieceType::NONE) break;
            }
            for (int down = index + 8; down < 64; down += 8) {
                if (_positions[down].getColor() != PieceColor::WHITE) moves.push_back(indexPosition.at(down));
                if (_positions[down].getType() != PieceType::NONE) break;
            }
            for (int left = index - 1; left >= 0 && left >= index - (index % 8); left -= 1) {
                if (_positions[left].getColor() != PieceColor::WHITE) moves.push_back(indexPosition.at(left));
                if (_positions[left].getType() != PieceType::NONE) break;
            }
            for (int right = index + 1; right <= 63 && right <= index + (7 - (index % 8)); right += 1) {
                if (_positions[right].getColor() != PieceColor::WHITE) moves.push_back(indexPosition.at(right));
                if (_positions[right].getColor() == PieceColor::BLACK) break;
            }
        }
        
    }

    return moves;
}