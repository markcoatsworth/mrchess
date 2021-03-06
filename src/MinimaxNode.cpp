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
    // After playing the move, calculate the board score for the playerColor
    //_score = _board.evaluateScore(_playerColor);
}

// TODO: Could this get merged into the MoveEngine::buildMinimaxTree function?
double MinimaxNode::getMinimaxScore() {

    // Base case
    if (_childNodes == nullptr) {
        _score = _board.evaluateScore();
        return _score;
    }

    // Recursive case
    // Start by retrieving score for all child nodes
    for (auto it = _childNodes->begin(); it != _childNodes->end(); it++) {
        (*it).getMinimaxScore();
    }

    // If there are no possible moves, this is a checkmate position.
    // For now, just return an unreasonably high score.
    // TODO: Find a better way to manage checkmates
    if (_childNodes->size() == 0) {
        // Because we care about the _isMaxLevel of the child nodes, without any child nodes we care about the inverse
        // So if this current level has _isMaxLevel == true, then return a very negative number, and vice verse
        if (_isMaxLevel) {
            return -10000;
        }
        else {
            return 10000;
        }
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
