#pragma once

#include "Board.h"
#include "Piece.h"

class MinimaxNode
{
public:
    MinimaxNode();
    MinimaxNode(PieceColor color, Board board, std::string move);
    void addChildMove(MinimaxNode childMove);
    std::string getBestMove();
    
    std::string getMove() {
        return _move;
    }
    int getScore() {
        return _score;
    }

private:
    Board _board;
    int _score;
    PieceColor _playerColor;
    std::list<MinimaxNode> _childMoves;
    std::string _move;
};
