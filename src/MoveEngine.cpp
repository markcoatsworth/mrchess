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
    std::unique_ptr<std::vector<MinimaxNode>> childNodes = std::make_unique<std::vector<MinimaxNode>>();
    std::vector<std::string> colorMoves = node->getBoard().getColorAvailableMoves(node->getColor());
    for (auto& move : colorMoves) {
        childNodes->push_back(MinimaxNode(color, node->getBoard(), move));
    }

    // Sort the child nodes; nodes are sorted by score
    std::sort(childNodes->begin(), childNodes->end());

    // Do the recursive call on all child nodes
    for (auto it = childNodes->begin(); it != childNodes->end(); it++) {
        Color opponentColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
        buildMinimaxTreeLevel(&*it, opponentColor, depth - 1);
    }

    // Lastly, move the childNodes vector to our node
    node->setChildNodes(std::move(childNodes));

    /*
    // Debug: print the list of child nodes
    std::cout << std::fixed << std::setprecision(4);
    std::string colorStr = (color == Color::BLACK) ? "BLACK" : "WHITE";
    cout << "Child nodes for color " << colorStr << " at depth " << depth << ": [ ";
    for (auto it = node->getChildNodes()->begin(); it != node->getChildNodes()->end(); it++) {
        cout << "{" << it->getMove() << ": " << it->getScore() << "}, ";
    }
    cout << " ]" << endl;;
    */
}

std::string MoveEngine::getMinimaxMove(Board board, Color color) {
    buildMinimaxTree(board, color);
    std::string minimaxMove = _minimaxTree.findBestMove();
    return minimaxMove;
}