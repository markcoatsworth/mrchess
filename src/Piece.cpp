#include <iostream>
#include <string>

#include "Piece.h"

using namespace std;

Piece::Piece()
{}

Piece::Piece(PieceColor color, PieceType type) {
    // Set the piece color. We don't need to set the color for BLACK, since
    // this is the default for any non-null piece.
    if (color == PieceColor::WHITE) {
        _piece |= _WHITE;
    }

    // Set the piece TYPE. Defaults to _NONE;
    switch (type) {
        case PieceType::PAWN:
            _piece |= _PAWN;
            break;
        case PieceType::KNIGHT:
            _piece |= _KNIGHT;
            break;
        case PieceType::BISHOP:
            _piece |= _BISHOP;
            break;
        case PieceType::ROOK:
            _piece |= _ROOK;
            break;
        case PieceType::QUEEN:
            _piece |= _QUEEN;
            break;
        case PieceType::KING:
            _piece |= _KING;
            break;
        default:
            break;
    }
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
    if ((_piece & _WHITE) == _WHITE) {
        return PieceColor::WHITE;
    }
    // A black piece has the color bit = 0, but some other bits are set
    else if ((_piece & _NONE) != _piece) {
        return PieceColor::BLACK;
    }
    else {
        return PieceColor::NONE;
    }
}

const PieceType Piece::getType() {
    if ((_piece & _PAWN) == _PAWN) {
        return PieceType::PAWN;
    }
    else if ((_piece & _KNIGHT) == _KNIGHT) {
        return PieceType::KNIGHT;
    }
    else if ((_piece & _BISHOP) == _BISHOP) {
        return PieceType::BISHOP;
    }
    else if ((_piece & _ROOK) == _ROOK) {
        return PieceType::ROOK;
    }
    else if ((_piece & _QUEEN) == _QUEEN) {
        return PieceType::QUEEN;
    }
    else if ((_piece & _KING) == _KING) {
        return PieceType::KING;
    }
    else {
        return PieceType::NONE;
    }
}