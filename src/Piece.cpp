#include <iostream>
#include <string>

#include "Piece.h"

using namespace std;

Piece::Piece()
{}

Piece::Piece(Color color, PieceType type) {
    // White piece have to set the second-right bit to 1. Black pieces leave it at 0.
    if (color == Color::WHITE) {
        _piece |= std::byte{ 0b00000010 };
    }
    _piece |= (std::byte)type;
}

Piece::Piece(const Piece& piece) {
    _piece = piece._piece;
}

std::string Piece::toString() {
    std::string str = ".";
 
    switch (getColor()) {
        case Color::WHITE:
            str = "W";
            break;
        case Color::BLACK:
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

const Color Piece::getColor() {
    // A white piece has the color bit = 1
    if ((_piece & std::byte{ 0b00000010 }) == std::byte{ 0b00000010 }) {
        return Color::WHITE;
    }
    // A black piece has the color bit = 0, but some other bits are set
    else if ((_piece | (std::byte)0) != (std::byte)0) {
        return Color::BLACK;
    }
    // A piece with color NONE is a piece that does not exist, ie. an empty square
    else {
        return Color::NONE;
    }
}

const PieceType Piece::getType() {
    return PieceType(_piece & std::byte{ 0b11111100 });
}

void Piece::setType(PieceType type) {
    // Remove the old type while preserving color/move status
    _piece &= std::byte{ 0b0000011 };
    // Set the new type
    _piece |= (std::byte)type;
}