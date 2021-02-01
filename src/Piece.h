#pragma once

#include <cstddef>
#include <string>

using namespace std;

enum class PieceColor { NONE, BLACK, WHITE };
enum class PieceType { NONE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

/*
 * Each piece is represented by a single byte, or eight bits.
 * The bits correspond to the following (left to right):
 * 1) has moved (0 = unmoved, 1 = moved)
 * 2) color (0 = black, 1 = white)
 * 3) 1 = is type Pawn
 * 4) 1 = is type Knight
 * 5) 1 = is type Bishop
 * 6) 1 = is type Rook
 * 7) 1 = is type Queen
 * 8) 1 = is type King
 *
 * A piece of value 0 (0x00000000) represents an empty piece, does not exist
 * 
 */

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
    const PieceColor getColor();
    
    /**
     * Return the type of piece
     */
    const PieceType getType();



  private:

      std::byte _piece{ 0b00000000 };

      inline static std::byte _NONE     { 0b00000000 };
      inline static std::byte _MOVED    { 0b00000001 };
      inline static std::byte _WHITE    { 0b00000010 };
      inline static std::byte _PAWN     { 0b00000100 };
      inline static std::byte _KNIGHT   { 0b00001000 };
      inline static std::byte _BISHOP   { 0b00010000 };
      inline static std::byte _ROOK     { 0b00100000 };
      inline static std::byte _QUEEN    { 0b01000000 };
      inline static std::byte _KING     { 0b10000000 };

};

