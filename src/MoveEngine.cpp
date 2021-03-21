#include <chrono>
#include <future>
#include <iomanip>
#include <iostream>
#include <thread>

#include "MinimaxNode.h"
#include "MoveEngine.h"

MoveEngine::MoveEngine() {

}

void MoveEngine::buildMinimaxTree(Board board, Color color) {
    int treeDepth = 5;
    //auto start = chrono::steady_clock::now();

    // The root of the tree is a single MinimaxNode used to start the recursive build
    _minimaxTree = MinimaxNode(board, Color::NONE,  "", false, treeDepth);
    buildMinimaxTreeLevel(&_minimaxTree, color, true, treeDepth - 1);

    //auto elapsed = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start).count();
    //cout << "Building minimax tree with depth " << treeDepth << " took " << elapsed << "us" << endl;
}

void MoveEngine::buildMinimaxTreeLevel(MinimaxNode* node, Color color, bool isMaxLevel, int depth) {

    // Base case
    if (depth == 0) {
        return;
    }

    // Recursive case
    std::unique_ptr<std::vector<MinimaxNode>> childNodes = std::make_unique<std::vector<MinimaxNode>>();
    //bool checkForExposedKing = (depth == 1) ? false : true;
    bool checkForExposedKing = true;
    std::vector<std::string> colorMoves = node->getBoard().getColorAvailableMoves(color, checkForExposedKing);
    for (auto& move : colorMoves) {
        Color opponentColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
        childNodes->push_back(MinimaxNode(node->getBoard(), color, move, isMaxLevel, depth));
        buildMinimaxTreeLevel(&childNodes->back(), opponentColor, !isMaxLevel, depth - 1);
    }
    node->setChildNodes(std::move(childNodes));

}

std::string MoveEngine::getMinimaxMove(Board board, Color color) {
    buildMinimaxTree(board, color);

    // Now recursively search the tree
    std::string minimaxMove = _minimaxTree.findBestMove();

    // If the computer is in checkmate, just return the checkmate move string
    if (minimaxMove == "!!") {
        return minimaxMove;
    }

    // Does this move put the opponent into a check position? If so indicate this in the move string.
    bool checksOpponent = board.doesMoveCheckOpponent(minimaxMove);
    if (checksOpponent) {
        minimaxMove += "!";
    }

    // If this move checks the opponent, does it also checkmate? If so indicate this in the move string.
    if (checksOpponent) {
        bool checkmatesOpponent = board.doesMoveCheckmateOpponent(minimaxMove);
        if (checkmatesOpponent) {
            minimaxMove += "!";
        }
    }

    return minimaxMove;
}

std::string MoveEngine::getRandomMove(Board board, Color color) {

    std::string move;
    std::vector<std::string> availableMoves = board.getColorAvailableMoves(color);

    // Seed a random number generator using current time milliseconds
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::srand(millis);

    // If no moves are available, return an empty string. This indicates checkmate.
    // TODO: Should checkmate be evaluated during an offensive move, instead of reactively?
    if (availableMoves.size() > 0) {
        auto it = availableMoves.begin();
        std::advance(it, std::rand() % availableMoves.size());
        move = *it;

        // Does this move put the opponent into a check position? If so indicate this in the move string.
        bool checksOpponent = board.doesMoveCheckOpponent(move);
        if (checksOpponent) {
            move += "!";
        }
    }

    return move;
}