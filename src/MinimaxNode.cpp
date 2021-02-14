#include <iostream>

#include "MinimaxNode.h"

MinimaxNode::MinimaxNode() :
    _playerColor(Color::NONE),
    _score(0),
    _childNodes(nullptr),
    _move("")
{
}

MinimaxNode::MinimaxNode(Board board, Color playerColor, std::string move, bool isMaxLevel) :
    _board(board),
    _isMaxLevel(isMaxLevel),
    _playerColor(playerColor),
    _score(0),
    _childNodes(nullptr),
    _move(move)
{
    // TODO: The following should probably not happen in the constructor?
    // We want to play the move after copying the board, not before
    if (!_move.empty()) {
        _board.playMove(_move);
    }
    // After playing the move, calculate the board score for the playerColor
    _score = _board.evaluateScore(_playerColor);
}

// TODO: Could this get merged into the MoveEngine::buildMinimaxTree function?
double MinimaxNode::getMinimaxScore() {

    // Base case
    if (_childNodes == nullptr) {
        return _score;
    }

    // Recursive case
    // Start by retrieving score for all child nodes
    for (auto it = _childNodes->begin(); it != _childNodes->end(); it++) {
        _score = (*it).getMinimaxScore();
    }

    // Now return the min or max score for all the child nodes
    bool isMaxLevel = _childNodes->front()._isMaxLevel;
    double minimaxScore = isMaxLevel ? 0 : 1;
    for (auto it = _childNodes->begin(); it != _childNodes->end(); it++) {
        if (isMaxLevel && it->_score > minimaxScore) {
            minimaxScore = it->_score;
        }
        else if (!isMaxLevel && it->_score < minimaxScore) {
            minimaxScore = it->_score;
        }
    }

    return minimaxScore;
}

std::string MinimaxNode::findBestMove() {
    double minimaxScore = getMinimaxScore();
    for (auto it = _childNodes->begin(); it != _childNodes->end(); it++) {
        if (it->_score == minimaxScore) {
            return it->_move;
        }
    }

    return _childNodes->back()._move;
}
