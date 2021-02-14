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
    _minimaxTree = MinimaxNode(board, color,  "", false);
    buildMinimaxTreeLevel(&_minimaxTree, color, true, treeDepth);
    
    // Debug: print the tree
    /*
    std::cout << std::fixed << std::setprecision(4);
    std::string colorStr = (color == Color::BLACK) ? "BLACK" : "WHITE";
    cout << "Child nodes count: " << _minimaxTree.getChildNodes()->size() << endl;
    cout << "Child nodes for color " << colorStr << ": [ ";
    for (auto it = _minimaxTree.getChildNodes()->begin(); it != _minimaxTree.getChildNodes()->end(); it++) {
        cout << "{" << it->getMove() << ": " << it->getScore() << "}, ";
    }
    cout << " ]" << endl;
    */

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
    std::vector<std::string> colorMoves = node->getBoard().getColorAvailableMoves(node->getColor());
    for (auto& move : colorMoves) {
        Color opponentColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
        childNodes->push_back(MinimaxNode(node->getBoard(), color, move, isMaxLevel));
        buildMinimaxTreeLevel(&childNodes->back(), opponentColor, !isMaxLevel, depth - 1);
    }
    node->setChildNodes(std::move(childNodes));

}

std::string MoveEngine::getMinimaxMove(Board board, Color color) {
    buildMinimaxTree(board, color);

    // Now recursively search the tree
    std::string minimaxMove = _minimaxTree.findBestMove();
    return minimaxMove;
}