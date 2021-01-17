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
    json requestJson;
    json responseJson;
    std::string requestMethod;
    std::string postData;

    // If this is a POST request, read + parse input stream data
    if (getenv("REQUEST_METHOD")) {
        requestMethod = getenv("REQUEST_METHOD");
        if (requestMethod == "POST") {
            std::ifstream is(stdin);
            char c;
            while (is.get(c)) {
                postData += c;
            }
            is.close();
            requestJson = json::parse(postData);
        }
    }

    // Setup the board
    board.setPositions(requestJson["positions"]);
    //board.draw();

    // Prepare response JSON
    responseJson["availableMoves"] = { "A1", "B2", "C3" };

    // Send response
    cout << "Content-type: application/json" << endl << endl;
    cout << responseJson.dump() << endl;

    return 0;
}
