#pragma once

#include "Board.h"
#include "Piece.h"

class MinimaxNode
{
public:

    /*
     * Default Minimax constructor
     * Important: we never want to use this! Use the expanded constructor that updates the board according to move
     */
    MinimaxNode();

    /*
     * MinimaxNode constructor sets the basic object according to the board and move provided
     */
    MinimaxNode(Board board, Color color, std::string move, bool isMaxLevel, int depth);

    /*
     * Returns the Minimax score for this level, making recursive calls as needed
     */
    double getMinimaxScore();

    /*
     * Finds the best move at this node level based on Minimax evaluation
     */
    std::string findBestMove();

    /*
     * Overloaded comparison operators for sorting
     */
    bool operator < (const MinimaxNode& node) const {
        return _score < node._score;
    }
    bool operator > (const MinimaxNode& node) const {
        return _score > node._score;
    }

    /*
     * Trivial getter and setter functions
     */
    Board getBoard() {
        return _board;
    }
    double getScore() {
        return _score;
    }
    Color getColor() {
        return _playerColor;
    }
    std::vector<MinimaxNode>* getChildNodes() {
        return _childNodes.get();
    }
    void setChildNodes(std::unique_ptr<std::vector<MinimaxNode>> childNodes) {
        _childNodes = std::move(childNodes);
    }
    std::string getMove() {
        return _move;
    }

private:
    std::string _move;
    double _score;
    bool _isMaxLevel;
    Color _playerColor;
    int _depth;
    Board _board;
    std::unique_ptr<std::vector<MinimaxNode>> _childNodes;
};
