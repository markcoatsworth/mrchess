// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "catch.hpp"
#include "../src/Board.h"
#include "../src/nlohmann/json.hpp"
#include "../src/MinimaxNode.h"
#include "../src/MoveEngine.h"

using json = nlohmann::json;
using namespace std;


TEST_CASE("Pawn legal moves") {
    Board board;
    json requestJson;
    json responseJson;

    SECTION("Pawn starting from the opening line") {
        requestJson["board"] = { {"b7", "black pawn"} };
        board.setPieces(requestJson["board"]);
        responseJson["moves"] = board.getPieceAvailableMoves("b7");
        REQUIRE(responseJson["moves"] == json({ "b7b6", "b7b5" }));
    }
    SECTION("Pawn after a first move") {
        requestJson["board"] = { {"b6", "black pawn"} };
        board.setPieces(requestJson["board"]);
        responseJson["moves"] = board.getPieceAvailableMoves("b6");
        REQUIRE(responseJson["moves"] == json({ "b6b5" }));
    }
    SECTION("Pawn taking an opponent piece") {
        requestJson["board"] = { {"b6", "black pawn"}, {"c5", "white pawn"} };
        board.setPieces(requestJson["board"]);
        responseJson["moves"] = board.getPieceAvailableMoves("b6");
        REQUIRE(responseJson["moves"] == json({ "b6b5", "b6c5" }));
    }
}

TEST_CASE("Knight legal moves") {
    Board board;
    json requestJson;
    json responseJson;

    SECTION("Knight starting from the opening line") {
        requestJson["board"] = { {"b8", "black knight"}, {"d7", "black pawn"} };
        board.setPieces(requestJson["board"]);
        responseJson["moves"] = board.getPieceAvailableMoves("b8");
        REQUIRE(responseJson["moves"] == json({ "b8a6", "b8c6" }));
    }
}

TEST_CASE("Simple 1-move checkmates") {
    Board board;
    json requestJson;
    json responseJson;
    MoveEngine moveEngine;

    SECTION("Two rooks checkmate") {
        requestJson["board"] = { {"e1", "white king"}, {"b2", "black rook"}, {"a8", "black rook"} };
        board.setPieces(requestJson["board"]);
        responseJson["move"] = moveEngine.getMinimaxMove(board, Color::BLACK);
        REQUIRE(responseJson["move"] == "a8a1!!");
    }

}

TEST_CASE("Famous games 1-move checkmates") {
    Board board;
    json requestJson;
    json responseJson;
    MoveEngine moveEngine;

    SECTION("Bobby Fischer Game of the Century 1956") {
        requestJson["board"] = { 
            {"b8", "white queen"},
            {"f7", "black pawn"}, {"g7", "black king"},
            {"c6", "black pawn"}, {"g6", "black pawn"},
            {"b5", "black pawn"}, {"d5", "white knight"}, {"h5", "black pawn"},
            {"b4", "black bishop"}, {"h4", "white pawn"},
            {"b3", "black bishop"}, {"c3", "black knight"},
            {"a2", "black rook"}, {"g2", "white pawn"},
            {"c1", "white king"}
        };
        board.setPieces(requestJson["board"]);
        responseJson["move"] = moveEngine.getMinimaxMove(board, Color::BLACK);
        // Bobby Fischer played a2c2, but b4a3 is also a valid checkmate
        bool checkmate = responseJson["move"] == "a2c2!!" || responseJson["move"] == "b4a3!!";
        REQUIRE(checkmate);
    }
    SECTION("Short vs Morozevich 2002") {
        requestJson["board"] = {
            {"a6", "black knight"}, {"e6", "black king"}, {"h6", "black pawn"},
            {"c5", "white knight"}, {"e5", "black bishop"}, {"g5", "black pawn"},
            {"e4", "white king"}, {"f4", "black pawn"}, {"g4", "white pawn"},
            {"f3", "white pawn"}, {"h3", "white pawn"}
        };
        board.setPieces(requestJson["board"]);
        responseJson["move"] = moveEngine.getMinimaxMove(board, Color::BLACK);
        REQUIRE(responseJson["move"] == "a6c5!!");
    }
    SECTION("The King is Trapped, Ghinda vs Stohl 1985") {
        requestJson["board"] = {
            {"a8", "white queen"},
            {"f7", "black pawn"}, {"g7", "black pawn"}, {"h7", "black king"},
            {"e6", "black pawn"}, {"h6", "black pawn"},
            {"d5", "black knight"},
            {"e4", "white king"},
            {"d3", "white rook"}, {"f3", "white pawn"},
            {"b2", "black queen"}, {"e2", "white knight"}, {"g2", "white pawn"}, {"h2", "white pawn"}
        };
        board.setPieces(requestJson["board"]);
        responseJson["move"] = moveEngine.getMinimaxMove(board, Color::BLACK);
        REQUIRE(responseJson["move"] == "f7f5!!");
    }


}
