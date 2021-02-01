#include <iostream>
#include <string>

#include "Piece.h"

using namespace std;

Piece::Piece()
{}

Piece::Piece(PieceColor color, PieceType type) {
    _piece |= (std::byte)color;
    _piece |= (std::byte)type;
}

Piece::Piece(const Piece& piece) {
    _piece = piece._piece;
}

std::string Piece::toString() {
    std::string str = ".";
 
    switch (getColor()) {
        case PieceColor::WHITE:
            str = "W";
            break;
        case PieceColor::BLACK:
            str = "B";
            break;
    }
    switch (getType()) {
        case PieceType::PAWN:
            str += "P";
            break;
        case PieceType::KNIGHT:
            str += "Kn";
            break;
        case PieceType::BISHOP:
            str += "B";
            break;
        case PieceType::ROOK:
            str += "R";
            break;
        case PieceType::QUEEN:
            str += "Q";
            break;
        case PieceType::KING:
            str += "Ki";
            break;
    }
    return str;
}

const PieceColor Piece::getColor() {
    if ((_piece & (std::byte)PieceColor::WHITE) == (std::byte)PieceColor::WHITE) {
        return PieceColor::WHITE;
    }
    // A black piece has the color bit = 0, but some other bits are set
    else if ((_piece | (std::byte)PieceColor::NONE) != (std::byte)PieceColor::NONE) {
        return PieceColor::BLACK;
    }
    else {
        return PieceColor::NONE;
    }
}

const PieceType Piece::getType() {
    return PieceType(_piece & std::byte{ 0b11111100 });
}