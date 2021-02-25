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
    int treeDepth = 3;
    //auto start = chrono::steady_clock::now();
    
    // The root of the tree is a single MinimaxNode used to start the recursive build
    _minimaxTree = MinimaxNode(board, Color::NONE,  "", false, treeDepth + 1);
    buildMinimaxTreeLevel(&_minimaxTree, color, true, treeDepth);
    
    //auto elapsed = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start).count();
    //cout << "Building minimax tree with depth " << treeDepth << " took " << elapsed << "us" << endl;
}

void MoveEngine::buildMinimaxTreeLevel(MinimaxNode* node, Color color, bool isMaxLevel, int depth) {

    // Base case
    if (depth == 0) {
        return;
    }

    // Recursive case.
    std::unique_ptr<std::vector<MinimaxNode>> childNodes = std::make_unique<std::vector<MinimaxNode>>();
    std::vector<std::string> colorMoves = node->getBoard().getColorAvailableMoves(color);
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
    int i = 0;
    return minimaxMove;
}