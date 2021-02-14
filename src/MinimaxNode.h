#pragma once

#include "Board.h"
#include "Piece.h"

class MinimaxNode
{
public:
    MinimaxNode();
    MinimaxNode(Color color, Board board, std::string move);
    std::string findBestMove();

    bool operator < (const MinimaxNode& node) const {
        return _score < node._score;
    }

    bool operator > (const MinimaxNode& node) const {
        return _score > node._score;
    }

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
    Board _board;
    double _score;
    Color _playerColor;
    std::unique_ptr<std::vector<MinimaxNode>> _childNodes;
    std::string _move;
    bool isMaxLevel;
};
