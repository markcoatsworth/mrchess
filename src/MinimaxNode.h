#pragma once

#include "Board.h"
#include "Piece.h"

class MinimaxNode
{
public:
    MinimaxNode();
    MinimaxNode(Color color, Board board, std::string move);
    MinimaxNode* addChildMove(MinimaxNode childMove);
    std::string getBestMove();
    
    Board getBoard() {
        return _board;
    }
    Color getColor() {
        return _playerColor;
    }
    std::string getMove() {
        return _move;
    }
    int getScore() {
        return _score;
    }

private:
    Board _board;
    int _score;
    Color _playerColor;
    std::list<MinimaxNode> _childMoves;
    std::string _move;
};
