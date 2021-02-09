#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#ifdef WIN32
    #include <windows.h>
    #include <debugapi.h>
#endif

#include "Board.h"
#include "nlohmann/json.hpp"
#include "MoveEngine.h"

using json = nlohmann::json;
using namespace std;

int main(int argc, char* argv[]) {

    Board board;
    bool debug = false;
    json requestJson;
    json responseJson;
    MoveEngine moveEngine;
    std::string requestAction;
    std::string requestColor;
    std::string requestMethod;
    std::string requestPosition;
    std::string postData;

    // Parse command-line arguments
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-debug") == 0) {
            debug = true;
        }
    }

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

            // Set local variables defined in the JSON request
            if (requestJson.find("action") != requestJson.end()) {
                requestAction = requestJson["action"];
            }
            if (requestJson.find("position") != requestJson.end()) {
                requestPosition = requestJson["position"];
            }
            if (requestJson.find("color") != requestJson.end()) {
                requestColor = requestJson["color"];
            }
        }
    }
    // If this is not a POST request, drop in some default values for debugging
    else {
        requestJson["board"] = {
            {"a8", "black rook"},
            {"b8", "black knight"},
            {"c8", "black bishop"},
            {"d8", "black queen"},
            {"e8", "black king"},
            {"f8", "black bishop"},
            {"g8", "black knight"},
            {"h8", "black rook"},
            {"a7", "black pawn"},
            {"b7", "black pawn"},
            {"c7", "black pawn"},
            {"d7", "black pawn"},
            {"e7", "black pawn"},
            {"f7", "black pawn"},
            {"g7", "black pawn"},
            {"h7", "black pawn"},
            {"a2", "white pawn"},
            {"b2", "white pawn"},
            {"c2", "white pawn"},
            {"d2", "white pawn"},
            {"e2", "white pawn"},
            {"f2", "white pawn"},
            {"g2", "white pawn"},
            {"h2", "white pawn"},
            {"a1", "white rook"},
            {"b1", "white knight"},
            {"c1", "white bishop"},
            {"d1", "white queen"},
            {"e1", "white king"},
            {"f1", "white bishop"},
            {"g1", "white knight"},
            {"h1", "white rook"}
        };
        requestAction = "getMove";
        requestColor = "black";
        requestPosition = "d2";
    }

#ifdef WIN32
    // Check if the request asked to wait for debugger to attach
    if (requestJson.find("debugger") != requestJson.end() && requestJson["debugger"] == true) {
        bool is_debugger = false;
        while (!is_debugger) {
            is_debugger = IsDebuggerPresent();
            std::this_thread::sleep_for(1000ms);
        }
    }
#endif

    // Setup the board
    board.setPieces(requestJson["board"]);

    // Perform the requested action
    if (requestAction == "getPieceAvailableMoves") {
        responseJson["moves"] = board.getPieceAvailableMoves(requestPosition);
    }
    else if (requestAction == "getMove") {
        Color color = (requestColor == "white") ? Color::WHITE : Color::BLACK;
        responseJson["move"] = moveEngine.getMinimaxMove(board, color);
    }

    // Send response
    cout << "Content-type: application/json" << endl << endl;
    cout << responseJson.dump() << endl;

    return 0;
}
