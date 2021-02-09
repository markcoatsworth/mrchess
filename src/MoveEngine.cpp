#include <chrono>
#include <future>
#include <iostream>
#include <thread>

#include "MinimaxNode.h"
#include "MoveEngine.h"

MoveEngine::MoveEngine() {

}

void MoveEngine::buildMinimaxTree(Board board, Color color) {
    int treeDepth = 3;
    //auto start = chrono::steady_clock::now();
    _minimaxTree = MinimaxNode(color, board, "");
    buildMinimaxTreeLevel(&_minimaxTree, color, treeDepth);
    //auto elapsed = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start).count();
    //cout << "Building minimax tree with depth " << treeDepth << " took " << elapsed << "us" << endl;
}

void MoveEngine::buildMinimaxTreeLevel(MinimaxNode* node, Color color, int depth) {

    // Base case
    if (depth == 0) {
        return;
    }

    // Recursive case.
    std::vector<std::string> colorMoves = node->getBoard().getColorAvailableMoves(node->getColor());
    for (auto& move : colorMoves) {
        Color opponentColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
        MinimaxNode* childNode = node->addChildMove(MinimaxNode(color, node->getBoard(), move));
        buildMinimaxTreeLevel(childNode, opponentColor, depth - 1);
    }
}

std::string MoveEngine::getMinimaxMove(Board board, Color color) {
    buildMinimaxTree(board, color);
    std::string minimaxMove = _minimaxTree.getBestMove();
    return minimaxMove;
}