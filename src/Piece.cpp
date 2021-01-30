#include <string>

#include "Piece.h"

using namespace std;

Piece::Piece(): 
    _color(PieceColor::NONE), 
    _type(PieceType::NONE) 
{}

Piece::Piece(PieceColor color, PieceType type):
    _color(color), 
    _type(type) 
{}

Piece::Piece(const Piece& piece) {
    _color = piece._color;
    _type = piece._type;
}

std::string Piece::toString() {
    std::string str = ".";
    switch (_color) {
        case PieceColor::WHITE: str = "W"; break;
        case PieceColor::BLACK: str = "B"; break;
    }
    switch (_type) {
        case PieceType::PAWN:   str += "P"; break;
        case PieceType::KNIGHT: str += "Kn"; break;
        case PieceType::BISHOP: str += "B"; break;
        case PieceType::ROOK:   str += "R"; break;
        case PieceType::QUEEN:  str += "Q"; break;
        case PieceType::KING:   str += "Ki"; break;
    }
    return str;
}

const std::string Piece::getPosition() {
    return "";
}

const PieceColor Piece::getColor() {
    return _color;
}

const PieceType Piece::getType() {
    return _type;
}