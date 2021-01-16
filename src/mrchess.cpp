#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

int main() {

	json requestJson;
	json responseJson;
	std::string requestMethod;
	std::string postData;

	// If this is a POST request, read + parse input stream data
	if (getenv("requestMethod")) {
		requestMethod = getenv("requestMethod");
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

	// Prepare response JSON
	responseJson["availableMoves"] = { "pos1", "pos2", "pos3" };

	// Send response
	cout << "Content-type: application/json" << endl << endl;
	cout << responseJson.dump() << endl;

	return 0;
}
