#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "Board.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

int main() {

    Board board;
    bool debug = false;
    json requestJson;
    json responseJson;
    std::string requestAction;
    std::string requestMethod;
    std::string requestPosition;
    std::string postData;

    // If this is a POST request, read + parse input stream data
    if (getenv("REQUEST_METHOD")) {
        requestMethod = getenv("REQUEST_METHOD");
        if (requestMethod == "POST") {
            std::getline(cin, postData);
            try {
                requestJson = json::parse(postData);
            }
            catch (json::parse_error error) {
                responseJson["parseError"] = error.what();
            }
            requestAction = requestJson["action"];
            requestPosition = requestJson["position"];
        }
    }
    // If this is not a POST request, assume we're in debug mode
    else {
        requestJson["board"] = {
            {"A8", "black rook"},
            {"B8", "black knight"},
            {"C8", "black bishop"},
            {"D8", "black queen"},
            {"E8", "black king"},
            {"F8", "black bishop"},
            {"G8", "black knight"},
            {"H8", "black rook"},
            {"A7", "black pawn"},
            {"B7", "black pawn"},
            {"C7", "black pawn"},
            {"D7", "black pawn"},
            {"E7", "black pawn"},
            {"F7", "black pawn"},
            {"G7", "black pawn"},
            {"H7", "black pawn"},
            {"A2", "white pawn"},
            {"B2", "white pawn"},
            {"C2", "white pawn"},
            {"D2", "white pawn"},
            {"E2", "white pawn"},
            {"F2", "white pawn"},
            {"G2", "white pawn"},
            {"H2", "white pawn"},
            {"A1", "white rook"},
            {"B1", "white knight"},
            {"C1", "white bishop"},
            {"D1", "white queen"},
            {"E1", "white king"},
            {"F1", "white bishop"},
            {"G1", "white knight"},
            {"D4", "white rook"}
        };
        requestAction = "getPieceAvailableMoves";
        requestPosition = "G1";
    }

    // Setup the board
    board.setPositions(requestJson["board"]);
    if (debug) {
        board.draw();
    }

    // Perform the requested action
    if (requestAction == "getPieceAvailableMoves") {
        responseJson = board.getPieceAvailableMoves(requestPosition);
    }

    // Send response
    cout << "Content-type: application/json" << endl << endl;
    cout << responseJson.dump() << endl;

    return 0;
}
