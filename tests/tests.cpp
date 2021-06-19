// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "catch.hpp"
#include "../src/Board.h"
#include "../src/nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;


TEST_CASE("Pawn legal moves") {
    Board board;
    json requestJson;
    json responseJson;

    // Pawn starting from the opening line
    requestJson["board"] = { {"b7", "black pawn"} };
    board.setPieces(requestJson["board"]);
    responseJson["moves"] = board.getPieceAvailableMoves("b7");
    REQUIRE(responseJson["moves"] == json({"b7b6", "b7b5"}));
    board.clear();

    // Pawn after a first move
    requestJson["board"] = { {"b6", "black pawn"} };
    board.setPieces(requestJson["board"]);
    responseJson["moves"] = board.getPieceAvailableMoves("b6");
    REQUIRE(responseJson["moves"] == json({"b6b5"}));
    board.clear();

    // Pawn taking an opponent piece
    requestJson["board"] = { {"b6", "black pawn"}, {"c5", "white pawn"} };
    board.setPieces(requestJson["board"]);
    responseJson["moves"] = board.getPieceAvailableMoves("b6");
    REQUIRE(responseJson["moves"] == json({ "b6b5", "b6c5" }));
    board.clear();
}