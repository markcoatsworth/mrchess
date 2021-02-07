#include <chrono>
#include <future>
#include <iostream>
#include <thread>

#include "MinimaxNode.h"
#include "MoveEngine.h"

MoveEngine::MoveEngine() {

}

void MoveEngine::buildMinimaxTree(Board board, PieceColor color, int depth) {
    std::vector<std::string> colorMoves = board.getColorAvailableMoves(color);
    for (auto const& move : colorMoves) {
        _minimaxTree.addChildMove(MinimaxNode(color, board, move));
    }
}

std::string MoveEngine::getMinimaxMove(Board board, PieceColor color) {
    buildMinimaxTree(board, color, 1);
    std::string minimaxMove = _minimaxTree.getBestMove();
    return minimaxMove;
}