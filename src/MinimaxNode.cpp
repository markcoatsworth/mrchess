#include "MinimaxNode.h"

MinimaxNode::MinimaxNode() :
    _board(Board()),
    _score(0),
    _playerColor(Color::NONE),
    _childNodes{},
    _move("")
{
}

MinimaxNode::MinimaxNode(Color playerColor, Board board, std::string move) :
    _board(board),
    _score(0),
    _playerColor(playerColor),
    _childNodes{},
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

std::string MinimaxNode::findBestMove() {
    return _childNodes->back().getMove();
}
