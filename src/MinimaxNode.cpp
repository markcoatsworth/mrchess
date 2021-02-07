#include "MinimaxNode.h"

MinimaxNode::MinimaxNode() :
    _board(Board()),
    _score(0),
    _playerColor(PieceColor::NONE),
    _childMoves{},
    _move("")
{
}

MinimaxNode::MinimaxNode(PieceColor playerColor, Board board, std::string move) :
    _board(board),
    _score(0),
    _playerColor(playerColor),
    _childMoves{},
    _move(move)
{
    // We want to play the move after copying the board, not before
    _board.playMove(_move);
    // After playing the move, calculate the board score for the playerColor
    _score = _board.evaluateScore(_playerColor);
}

void MinimaxNode::addChildMove(MinimaxNode childMove) {
    _childMoves.push_back(childMove);
}

std::string MinimaxNode::getBestMove() {
    std::string bestMove;
    int bestMoveScore = 0;

    for (auto it = _childMoves.begin(); it != _childMoves.end(); it++) {
        if (it->getScore() > bestMoveScore) {
            bestMove = it->getMove();
            bestMoveScore = it->getScore();
        }
    }

    return bestMove;
}
