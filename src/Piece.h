#pragma once

#include <cstddef>
#include <string>

using namespace std;

enum class Color { 
    NONE,
    BLACK,
    WHITE
};
enum class PieceType { 
    NONE = 0,
    PAWN = 4,
    KNIGHT = 8,
    BISHOP = 16,
    ROOK = 32,
    QUEEN = 64,
    KING = 128
};

class Piece
{
public:

    /**
     * Piece object constructor
     */
    Piece();

    /**
     * Piece object constructor with initialialized values
     */
    Piece(Color color, PieceType type);

    /**
     * Copy constructor
     */
    Piece(const Piece& piece);

    /**
     * Return a basic string representation of piece in the form "CT"
     * (C = color, T = tyoe), or "." if none
     */
    std::string toString();

    /**
     * Return the color of piece
     */
    const Color getColor();
    
    /**
     * Return the type of piece
     */
    const PieceType getType();

    /**
     * Set the type of piece (only used for promotion)
     */
    void setType(PieceType type);



  private:

    // Each piece is represented by a single byte
    // A null byte (all bits 0) represents a non-existant piece
    // The right-most bit shows if the piece has been moved (0 = not moved, 1 = moved)
    // The second-right bit represents the color (0 = black, 1 = white)
    // The left 6 bits represent the piece type, as per the PieceType enum
    std::byte _piece{ 0b00000000 };

};

