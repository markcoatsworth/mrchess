#pragma once

#include <string>

using namespace std;

enum class PieceColor { NONE, BLACK, WHITE };
enum class PieceType { NONE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

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
    Piece(PieceColor color, PieceType type);

    /**
     * Copy structor
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
    const PieceColor getColor();
    
    /**
     * Return the type of piece
     */
    const PieceType getType();



  private:

    PieceColor _color;
    PieceType _type;

};
