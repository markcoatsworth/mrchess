#include <iostream>

#include "MinimaxNode.h"

MinimaxNode::MinimaxNode() :
    _move(""),
    _score(0),
    _playerColor(Color::NONE),
    _childNodes(nullptr)
{
}

MinimaxNode::MinimaxNode(Board board, Color playerColor, std::string move, bool isMaxLevel, int depth) :
    _move(move),
    _score(0),
    _isMaxLevel(isMaxLevel),
    _playerColor(playerColor),
    _depth(depth),
    _board(board),
    _childNodes(nullptr)
{
    // TODO: The following should probably not happen in the constructor?
    // We want to play the move after copying the board, not before
    if (!_move.empty()) {
        _board.playMove(_move);
    }
}

// TODO: Could this get merged into the MoveEngine::buildMinimaxTree function?
double MinimaxNode::getMinimaxScore() {

    // Base case: leaf node
    if (_childNodes == nullptr) {
        _score = _board.evaluateScore();
        return _score;
    }

    // Base case: checkmate
    // We can tell this apart from a leaf node because _childNodes is initialized with size 0
    if (_childNodes->size() == 0) {
        _score = _isMaxLevel ? 10000 : -10000;
        return _score;
    }

    // Recursive case
    // Start by retrieving score for all child nodes
    for (auto it = _childNodes->begin(); it != _childNodes->end(); it++) {
        (*it).getMinimaxScore();
    }

    // Now return the min or max score for all the child nodes
    bool isMaxLevel = _childNodes->front()._isMaxLevel;
    double minimaxScore = _childNodes->front()._score;
    for (auto it = _childNodes->begin(); it != _childNodes->end(); it++) {
        if (isMaxLevel && it->_score > minimaxScore) {
            minimaxScore = it->_score;
        }
        else if (!isMaxLevel && it->_score < minimaxScore) {
            minimaxScore = it->_score;
        }
    }

    _score = minimaxScore;
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
