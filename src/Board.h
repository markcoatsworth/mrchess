#pragma once

#include <cstddef>
#include <unordered_map>

#include "nlohmann/json.hpp"
#include "Piece.h"

using json = nlohmann::json;
using namespace std;

enum class MoveStrategy {
    RANDOM,
    MINMAX
};

class Board
{
public:

    /**
     * Constructs a basic Board object
     */
    Board();

    /**
     * Constructs a Board object initialized with a set of positions
     */
    Board(json& positions);

    /**
     * Copy constructor
     */
    Board(const Board& board);
    
    /**
     * Board object destructor. Do we actually need this?
     */
    ~Board();

    /**
      Draw an ASCII representation of the board. Used for debugging.
     */
    void draw();

    /**
     * Set all the piece positions on the board
     */
    void setPieces(json &piecePositions);

    /**
     * Get a list of all available moves for the specified color
     */
    std::vector<std::string> getColorAvailableMoves(Color color, bool checkForExposedKing = true);

    /**
     * Get a list of all available moves for the piece at specified position
     */
    std::vector<std::string> getPieceAvailableMoves(std::string position, bool checkForExposedKing = true);

    /**
     * Evaluates the board score
     * Returns a positive value for black advantage (higher value means greater advantage)
     * Returns a negative value for white advatage (lower value means greater advantage)
     */
    double evaluateScore();

    /**
     * Play the given move, update board data structures accordingly
     */
    void playMove(std::string move);

    /**
     * Determine if this board is in the check, if the specified color king is exposed
     */
    bool isInCheck(Color checkColor);

    /**
     * Determines if a given move puts the opponent in check
     */
    bool doesMoveCheckOpponent(std::string move);

    /**
     * Determines if a given move exposes the player's king
     */
    bool doesMoveExposeKing(std::string move);

private:

    /**
     * Map of chess positions (ie. a2, d4) to board array indices (48, 35)
     * Hardcoded for O(1) lookups
     */
    static inline const std::unordered_map<std::string, int> positionIndex = {
        {"a8", 0},  {"b8", 1},  {"c8", 2},  {"d8", 3},  {"e8", 4},  {"f8", 5},  {"g8", 6},  {"h8", 7},
        {"a7", 8},  {"b7", 9},  {"c7", 10}, {"d7", 11}, {"e7", 12}, {"f7", 13}, {"g7", 14}, {"h7", 15},
        {"a6", 16}, {"b6", 17}, {"c6", 18}, {"d6", 19}, {"e6", 20}, {"f6", 21}, {"g6", 22}, {"h6", 23},
        {"a5", 24}, {"b5", 25}, {"c5", 26}, {"d5", 27}, {"e5", 28}, {"f5", 29}, {"g5", 30}, {"h5", 31},
        {"a4", 32}, {"b4", 33}, {"c4", 34}, {"d4", 35}, {"e4", 36}, {"f4", 37}, {"g4", 38}, {"h4", 39},
        {"a3", 40}, {"b3", 41}, {"c3", 42}, {"d3", 43}, {"e3", 44}, {"f3", 45}, {"g3", 46}, {"h3", 47},
        {"a2", 48}, {"b2", 49}, {"c2", 50}, {"d2", 51}, {"e2", 52}, {"f2", 53}, {"g2", 54}, {"h2", 55},
        {"a1", 56}, {"b1", 57}, {"c1", 58}, {"d1", 59}, {"e1", 60}, {"f1", 61}, {"g1", 62}, {"h1", 63}
    };

    /**
     * Map of array indicies (ie. 48, 35) to chess positions (a2, d4)
     * Hardcoded for O(1) lookups
     */
    static inline const std::unordered_map<int, std::string> indexPosition = {
        {0, "a8"},  {1, "b8"},  {2, "c8"},  {3, "d8"},  {4, "e8"},  {5, "f8"},  {6, "g8"},  {7, "h8"},
        {8, "a7"},  {9, "b7"},  {10, "c7"}, {11, "d7"}, {12, "e7"}, {13, "f7"}, {14, "g7"}, {15, "h7"},
        {16, "a6"}, {17, "b6"}, {18, "c6"}, {19, "d6"}, {20, "e6"}, {21, "f6"}, {22, "g6"}, {23, "h6"},
        {24, "a5"}, {25, "b5"}, {26, "c5"}, {27, "d5"}, {28, "e5"}, {29, "f5"}, {30, "g5"}, {31, "h5"},
        {32, "a4"}, {33, "b4"}, {34, "c4"}, {35, "d4"}, {36, "e4"}, {37, "f4"}, {38, "g4"}, {39, "h4"},
        {40, "a3"}, {41, "b3"}, {42, "c3"}, {43, "d3"}, {44, "e3"}, {45, "f3"}, {46, "g3"}, {47, "h3"},
        {48, "a2"}, {49, "b2"}, {50, "c2"}, {51, "d2"}, {52, "e2"}, {53, "f2"}, {54, "g2"}, {55, "h2"},
        {56, "a1"}, {57, "b1"}, {58, "c1"}, {59, "d1"}, {60, "e1"}, {61, "f1"}, {62, "g1"}, {63, "h1"}
    };

    std::array<Piece, 64> _pieces{ {} };

};

