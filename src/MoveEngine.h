#pragma once
#include "Board.h"
#include "MinimaxNode.h"
#include "Piece.h"


class MoveEngine
{
public:

    MoveEngine();

    /*
     * Wrapper function to build the MinimaxNode tree
     */
    void buildMinimaxTree(Board board, Color color);

    /*
     * Recursive function to build the MinimaxNode tree according to depth
     */
    void buildMinimaxTreeLevel(MinimaxNode* node, Color color, int depth = 0);

    /*
     * Get the next move from the MinimaxNode tree
     */
    std::string getMinimaxMove(Board board, Color color);

private:

    MinimaxNode _minimaxTree;
};

