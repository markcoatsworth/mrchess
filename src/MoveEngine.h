#pragma once
#include "Board.h"
#include "MinimaxNode.h"
#include "Piece.h"


class MoveEngine
{
public:

    MoveEngine();

    void buildMinimaxTree(Board board, PieceColor color, int depth);

    std::string getMinimaxMove(Board board, PieceColor color);

private:

    MinimaxNode _minimaxTree;
};

