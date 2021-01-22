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

using json = nlohmann::json;
using namespace std;

int main(int argc, char* argv[]) {

    Board board;
    bool debug = false;
    json requestJson;
    json responseJson;
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

#ifdef WIN32
    // If in debug mode, wait for the debugger to attach
    if (debug || requestJson.find("debug") != requestJson.end()) {
        bool is_debugger = false;
        while (!is_debugger) {
            is_debugger = IsDebuggerPresent();
            std::this_thread::sleep_for(1000ms);
        }
    }
#endif

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
    // If this is not a POST request, switch into debug mode
    else {
        debug = true;
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
            {"H1", "white rook"}
        };
        requestAction = "getPieceAvailableMoves";
        requestColor = "black";
        requestPosition = "G1";
    }

    // Setup the board
    board.setPositions(requestJson["board"]);

    // Perform the requested action
    if (requestAction == "getPieceAvailableMoves") {
        responseJson = board.getPieceAvailableMoves(requestPosition);
    }
    else if (requestAction == "getMove") {
        PieceColor color = (requestColor == "white") ? PieceColor::WHITE : PieceColor::BLACK;
        responseJson = board.getMove(color);
    }

    // Send response
    cout << "Content-type: application/json" << endl << endl;
    cout << responseJson.dump() << endl;

    return 0;
}
