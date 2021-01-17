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
    for (int i = 0; i < _positions.size(); i++) {
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