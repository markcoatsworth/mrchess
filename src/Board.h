#pragma once

#include <unordered_map>

#include "nlohmann/json.hpp"
#include "Piece.h"

using json = nlohmann::json;
using namespace std;

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
    Board(json &positions);
        
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
    void setPositions(json &positions);

    /**
     * Get a list of all available moves for the piece at specified position
     */
    std::vector<std::string> getPieceAvailableMoves(std::string position);

    /**
     * Map of chess positions (ie. A2, D4) to board array indices (48, 35)
     * Hardcoded for O(1) lookups
     */
    const std::unordered_map<std::string, int> positionIndex = {
        {"A8", 0},  {"B8", 1},  {"C8", 2},  {"D8", 3},  {"E8", 4},  {"F8", 5},  {"G8", 6},  {"H8", 7},
        {"A7", 8},  {"B7", 9},  {"C7", 10}, {"D7", 11}, {"E7", 12}, {"F7", 13}, {"G7", 14}, {"H7", 15},
        {"A6", 16}, {"B6", 17}, {"C6", 18}, {"D6", 19}, {"E6", 20}, {"F6", 21}, {"G6", 22}, {"H6", 23},
        {"A5", 24}, {"B5", 25}, {"C5", 26}, {"D5", 27}, {"E5", 28}, {"F5", 29}, {"G5", 30}, {"H5", 31},
        {"A4", 32}, {"B4", 33}, {"C4", 34}, {"D4", 35}, {"E4", 36}, {"F4", 37}, {"G4", 38}, {"H4", 39},
        {"A3", 40}, {"B3", 41}, {"C3", 42}, {"D3", 43}, {"E3", 44}, {"F3", 45}, {"G3", 46}, {"H3", 47},
        {"A2", 48}, {"B2", 49}, {"C2", 50}, {"D2", 51}, {"E2", 52}, {"F2", 53}, {"G2", 54}, {"H2", 55},
        {"A1", 56}, {"B1", 57}, {"C1", 58}, {"D1", 59}, {"E1", 60}, {"F1", 61}, {"G1", 62}, {"H1", 63}
    };

    /**
     * Map of array indicies (ie. 48, 35) to chess positions (A2, D4)
     * Hardcoded for O(1) lookups
     */
    const std::unordered_map<int, std::string> indexPosition = {
        {0, "A8"},  {1, "B8"},  {2, "C8"},  {3, "D8"},  {4, "E8"},  {5, "F8"},  {6, "G8"},  {7, "H8"},
        {8, "A7"},  {9, "B7"},  {10, "C7"}, {11, "D7"}, {12, "E7"}, {13, "F7"}, {14, "G7"}, {15, "H7"},
        {16, "A6"}, {17, "B6"}, {18, "C6"}, {19, "D6"}, {20, "E6"}, {21, "F6"}, {22, "G6"}, {23, "H6"},
        {24, "A5"}, {25, "B5"}, {26, "C5"}, {27, "D5"}, {28, "E5"}, {29, "F5"}, {30, "G5"}, {31, "H5"},
        {32, "A4"}, {33, "B4"}, {34, "C4"}, {35, "D4"}, {36, "E4"}, {37, "F4"}, {38, "G4"}, {39, "H4"},
        {40, "A3"}, {41, "B3"}, {42, "C3"}, {43, "D3"}, {44, "E3"}, {45, "F3"}, {46, "G3"}, {47, "H3"},
        {48, "A2"}, {49, "B2"}, {50, "C2"}, {51, "D2"}, {52, "E2"}, {53, "F2"}, {54, "G2"}, {55, "H2"},
        {56, "A1"}, {57, "B1"}, {58, "C1"}, {59, "D1"}, {60, "E1"}, {61, "F1"}, {62, "G1"}, {63, "H1"}
    };

  private:

      std::array<Piece, 64> _positions{ {} };

};

