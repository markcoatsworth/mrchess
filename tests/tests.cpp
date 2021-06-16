// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "catch.hpp"
#include "../src/Board.h"
#include "../src/Piece.h"
#include "../src/nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;


int Factorial(int number) {
    //return number <= 1 ? number : Factorial(number - 1) * number;  // fail
    return number <= 1 ? 1      : Factorial( number - 1 ) * number;  // pass
}

json getAvailableMoves(json request) {
    json response;
    return response;
}

TEST_CASE("Rook legal moves", "[single-file]") {
    Board board;
    json requestJson;
    requestJson["board"] = {
            {"b7", "black rook"},
    };
    board.setPieces(requestJson["board"]);
    json responseJson;
    responseJson["moves"] = board.getPieceAvailableMoves("b7");
}

TEST_CASE("Factorial of 0 is 1 (fail)", "[single-file]") {
    REQUIRE(Factorial(0) == 1);
}

TEST_CASE("Factorials of 1 and higher are computed (pass)", "[single-file]") {
    REQUIRE(Factorial(1) == 1);
    REQUIRE(Factorial(2) == 2);
    REQUIRE(Factorial(3) == 6);
    REQUIRE(Factorial(10) == 3628800);
}

// Compile & run:
// - g++ -std=c++11 -Wall -I$(CATCH_SINGLE_INCLUDE) -o 010-TestCase 010-TestCase.cpp && 010-TestCase --success
// - cl -EHsc -I%CATCH_SINGLE_INCLUDE% 010-TestCase.cpp && 010-TestCase --success

// Expected compact output (all assertions):
//
// prompt> 010-TestCase --reporter compact --success
// 010-TestCase.cpp:14: failed: Factorial(0) == 1 for: 0 == 1
// 010-TestCase.cpp:18: passed: Factorial(1) == 1 for: 1 == 1
// 010-TestCase.cpp:19: passed: Factorial(2) == 2 for: 2 == 2
// 010-TestCase.cpp:20: passed: Factorial(3) == 6 for: 6 == 6
// 010-TestCase.cpp:21: passed: Factorial(10) == 3628800 for: 3628800 (0x375f00) == 3628800 (0x375f00)
// Failed 1 test case, failed 1 assertion.