#include <chrono>
#include <ctime>
#include <fstream>
#include <future>
#include <iostream>
#include <thread>

#include "Board.h"
#include "nlohmann/json.hpp"
#include "Piece.h"

using json = nlohmann::json;
using namespace std;

/* MOVE REPRESENTATION
 * *******************
 * All moves are represented by a std::string object
 *   - The first two characters represent the move-from position (ie. "a2", "g5")
 *   - The next two characters represent the move-to position
 *   - Any characters following these represent a special move:
 *       'c' indicates a castling move
 *       'p' indicates promotion. Promotions always go to a queen.
 *       '!' indicates check
 *       '!!' indicates checkmate
 */

Board::Board() {

}

Board::Board(json &positions) {
    setPieces(positions);
}

Board::Board(const Board& board) {
    for (int i = 0; i < 64; i++) {
        _pieces[i] = board._pieces[i];
    }
}

Board::~Board() {

}

void Board::clear() {
    _pieces = {};
}

void Board::draw() {
    for (unsigned int i = 0; i < _pieces.size(); i++) {
        cout << _pieces[i].toString() << "\t";
        if ((i + 1) % 8 == 0) {
            cout << endl;
        }
    }
    cout << endl;
}

void Board::setPieces(json & piecePositions) {
    for (json::iterator it = piecePositions.begin(); it != piecePositions.end(); ++it) {
        std::string pieceClass = it.value();
        std::string piecePosition = it.key();

        Color color{};
        if (pieceClass.find("black") != std::string::npos) {
            color = Color::BLACK;
        }
        if (pieceClass.find("white") != std::string::npos) {
            color = Color::WHITE;
        }

        PieceType type{};
        if (pieceClass.find("pawn") != std::string::npos) {
            type = PieceType::PAWN;
        }
        if (pieceClass.find("knight") != std::string::npos) {
            type = PieceType::KNIGHT;
        }
        if (pieceClass.find("bishop") != std::string::npos) {
            type = PieceType::BISHOP;
        }
        if (pieceClass.find("rook") != std::string::npos) {
            type = PieceType::ROOK;
        }
        if (pieceClass.find("queen") != std::string::npos) {
            type = PieceType::QUEEN;
        }
        if (pieceClass.find("king") != std::string::npos) {
            type = PieceType::KING;
        }
        
        Piece piece = Piece(color, type);
        _pieces[positionIndex.at(piecePosition)] = piece;
    }
}

std::vector<std::string> Board::getColorAvailableMoves(Color color, bool checkForExposedKing) {
    std::vector<std::string> allMoves{};
    for (auto it = _pieces.begin(); it != _pieces.end(); it++) {
        if (it->getColor() == color) {
            std::string position = indexPosition.at(it - _pieces.begin());
            std::vector<std::string> thisPosMoves = getPieceAvailableMoves(position, checkForExposedKing);
            for (auto& move : thisPosMoves) {
                allMoves.push_back(move);
            }
        }
    }
    return allMoves;
}

std::vector<std::string> Board::getPieceAvailableMoves(std::string position, bool checkForExposedKing) {
    int index = positionIndex.at(position);
    Piece piece = _pieces[index];
    Color opponentColor = (piece.getColor() == Color::WHITE) ? Color::BLACK : Color::WHITE;
    std::vector<std::string> moves{};

    // Pawns need movement rules specific to each color (since they move in opposite directions)
    if (piece.getColor() == Color::WHITE && piece.getType() == PieceType::PAWN) {
        if (index >= 8 && _pieces[index - 8].getType() == PieceType::NONE) {
            if (index <= 16) { 
                moves.push_back(position + indexPosition.at(index - 8) + "p");
            }
            else {
                moves.push_back(position + indexPosition.at(index - 8));
            }
        }
        if (index >= 48 && index <= 55 && _pieces[index - 8].getType() == PieceType::NONE && _pieces[index - 16].getType() == PieceType::NONE) {
            moves.push_back(position + indexPosition.at(index - 16));
        }
        if (index >= 8 && (index % 8) <= 6 && _pieces[index - 7].getColor() == opponentColor) {
            if (index <= 16) {
                moves.push_back(position + indexPosition.at(index - 7) + "p");
            }
            else {
                moves.push_back(position + indexPosition.at(index - 7));
            }
        }
        if (index >= 9 && (index % 8) >= 1 && _pieces[index - 9].getColor() == opponentColor) {
            if (index <= 16) {
                moves.push_back(position + indexPosition.at(index - 9) + "p");
            }
            else {
                moves.push_back(position + indexPosition.at(index - 9));
            }
        }
    }
    else if (piece.getColor() == Color::BLACK && piece.getType() == PieceType::PAWN) {
        if (index <= 55 && _pieces[index + 8].getType() == PieceType::NONE) {
            if (index >= 48) {
                moves.push_back(position + indexPosition.at(index + 8) + "p");
            }
            else {
                moves.push_back(position + indexPosition.at(index + 8));
            }
        }
        if (index >= 8 && index <= 15 && _pieces[index + 8].getType() == PieceType::NONE && _pieces[index + 16].getType() == PieceType::NONE) {
            moves.push_back(position + indexPosition.at(index + 16));
        }
        if (index <= 55 && (index % 8) >= 1 && _pieces[index + 7].getColor() == opponentColor) {
            if (index >= 48) {
                moves.push_back(position + indexPosition.at(index + 7) + "p");
            }
            else {
                moves.push_back(position + indexPosition.at(index + 7));
            }
        }
        if (index <= 54 && (index % 8) <= 6 && _pieces[index + 9].getColor() == opponentColor) {
            if (index >= 48) {
                moves.push_back(position + indexPosition.at(index + 9) + "p");
            }
            else {
                moves.push_back(position + indexPosition.at(index + 9));
            }
        }
    }
    // All other pieces have the same movement rules, regardless of color
    else if (piece.getType() == PieceType::KNIGHT) {
        if ((index - 17) >= 0 && (index % 8) >= 1 && _pieces[index - 17].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index - 17));
        }
        if ((index - 15) >= 0 && (index % 8) <= 6 && _pieces[index - 15].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index - 15));
        }
        if ((index - 10) >= 0 && (index % 8) >= 2 && _pieces[index - 10].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index - 10));
        }
        if ((index - 6) >= 0 && (index % 8) <= 5 && _pieces[index - 6].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index - 6));
        }
        if ((index + 6) < 64 && (index % 8) >= 2 && _pieces[index + 6].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index + 6));
        }
        if ((index + 10) < 64 && (index % 8) <= 5 && _pieces[index + 10].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index + 10));
        }
        if ((index + 15) < 64 && (index % 8) >= 1 && _pieces[index + 15].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index + 15));
        }
        if ((index + 17) < 64 && (index % 8) <= 6 && _pieces[index + 17].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index + 17));
        }
    }
    else if (piece.getType() == PieceType::ROOK) {
        for (int up = index - 8; up >= 0; up -= 8) {
            if (_pieces[up].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(up));
            }
            if (_pieces[up].getColor() != Color::NONE) {
                break;
            }
        }
        for (int down = index + 8; down < 64; down += 8) {
            if (_pieces[down].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(down));
            }
            if (_pieces[down].getColor() != Color::NONE) {
                break;
            }
        }
        for (int left = index - 1; left >= 0 && left >= index - (index % 8); left -= 1) {
            if (_pieces[left].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(left));
            }
            if (_pieces[left].getColor() != Color::NONE) {
                break;
            }
        }
        for (int right = index + 1; right <= 63 && right <= index + (7 - (index % 8)); right += 1) {
            if (_pieces[right].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(right));
            }
            if (_pieces[right].getColor() != Color::NONE) {
                break;
            }
        }
    }
    else if (piece.getType() == PieceType::BISHOP) {
        for (int upleft = index - 9; upleft > 0 && (upleft % 8) != 7; upleft -= 9) {
            if (_pieces[upleft].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(upleft));
            }
            if (_pieces[upleft].getColor() != Color::NONE) {
                break;
            }
        }
        for (int upright = index - 7; upright > 0 && (upright % 8) != 0; upright -= 7) {
            if (_pieces[upright].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(upright));
            }
            if (_pieces[upright].getColor() != Color::NONE) {
                break;
            }
        }
        for (int downleft = index + 7; downleft < 64 && (downleft % 8) != 7; downleft += 7) {
            if (_pieces[downleft].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(downleft));
            }
            if (_pieces[downleft].getColor() != Color::NONE) {
                break;
            }
        }
        for (int downright = index + 9; downright < 64 && (downright % 8) != 0; downright += 9) {
            if (_pieces[downright].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(downright));
            }
            if (_pieces[downright].getColor() != Color::NONE) {
                break;
            }
        }
    }
    else if (piece.getType() == PieceType::QUEEN) {
        for (int upleft = index - 9; upleft >= 0 && (upleft % 8) != 7; upleft -= 9) {
            if (_pieces[upleft].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(upleft));
            }
            if (_pieces[upleft].getColor() != Color::NONE) {
                break;
            }
        }
        for (int up = index - 8; up >= 0; up -= 8) {
            if (_pieces[up].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(up));
            }
            if (_pieces[up].getColor() != Color::NONE) {
                break;
            }
        }
        for (int upright = index - 7; upright > 0 && (upright % 8) != 0; upright -= 7) {
            if (_pieces[upright].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(upright));
            }
            if (_pieces[upright].getColor() != Color::NONE) {
                break;
            }
        }
        for (int left = index - 1; left >= index - (index % 8); left -= 1) {
            if (_pieces[left].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(left));
            }
            if (_pieces[left].getColor() != Color::NONE) {
                break;
            }
        }
        for (int right = index + 1; right <= index + (7 - (index % 8)); right += 1) {
            if (_pieces[right].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(right));
            }
            if (_pieces[right].getColor() != Color::NONE) {
                break;
            }
        }
        for (int downleft = index + 7; downleft < 64 && (downleft % 8) != 7; downleft += 7) {
            if (_pieces[downleft].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(downleft));
            }
            if (_pieces[downleft].getColor() != Color::NONE) {
                break;
            }
        }
        for (int down = index + 8; down < 64; down += 8) {
            if (_pieces[down].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(down));
            }
            if (_pieces[down].getColor() != Color::NONE) {
                break;
            }
        }
        for (int downright = index + 9; downright < 64 && (downright % 8) != 0; downright += 9) {
            if (_pieces[downright].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(downright));
            }
            if (_pieces[downright].getColor() != Color::NONE) {
                break;
            }
        }
    }
    else if (piece.getType() == PieceType::KING) {
        if (index - 9 > 0 && index % 8 != 0 && _pieces[index - 9].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index - 9));
        }
        if (index - 8 > 0 && _pieces[index - 8].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index - 8));
        }
        if (index - 7 > 0 && index % 8 != 7 && _pieces[index - 7].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index - 7));
        }
        if (index - 1 > 0 && index % 8 != 0 && _pieces[index - 1].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index - 1));
        }
        if (index + 1 < 64 && index % 8 != 7 && _pieces[index + 1].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index + 1));
        }
        if (index + 7 < 64 && index % 8 != 0 && _pieces[index + 7].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index + 7));
        }
        if (index + 8 < 64 && _pieces[index + 8].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index + 8));
        }
        if (index + 9 < 64 && index % 8 != 7 && _pieces[index + 9].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index + 9));
        }
        // Castling moves
        // TODO: Need to add verification that the king and rook have never moved before
        if (piece.getColor() == Color::BLACK && index == 4) {
            if (_pieces[0].getColor() == Color::BLACK && _pieces[0].getType() == PieceType::ROOK
                    && _pieces[1].getType() == PieceType::NONE && _pieces[2].getType() == PieceType::NONE
                    && _pieces[3].getType() == PieceType::NONE) {
                moves.push_back(position + indexPosition.at(2) + "c");
            }
            else if (_pieces[7].getColor() == Color::BLACK && _pieces[7].getType() == PieceType::ROOK
                    && _pieces[5].getType() == PieceType::NONE && _pieces[6].getType() == PieceType::NONE) {
                moves.push_back(position + indexPosition.at(6) + "c");
            }
        }
        else if (piece.getColor() == Color::WHITE && index == 60) {
            if (_pieces[56].getColor() == Color::WHITE && _pieces[56].getType() == PieceType::ROOK
                    && _pieces[57].getType() == PieceType::NONE && _pieces[58].getType() == PieceType::NONE
                    && _pieces[59].getType() == PieceType::NONE) {
                moves.push_back(position + indexPosition.at(58) + "c");
            }
            else if (_pieces[63].getColor() == Color::WHITE && _pieces[63].getType() == PieceType::ROOK
                    && _pieces[61].getType() == PieceType::NONE && _pieces[62].getType() == PieceType::NONE) {
                moves.push_back(position + indexPosition.at(62) + "c");
            }
        }
    }

    // If desired, make sure this move does not put our king in check! If it does, remove from the list
    // Now launch the check function as a thread
    // Syntax for creating a new thread:
    // https://thispointer.com/c11-start-thread-by-member-function-with-arguments/
    // https://stackoverflow.com/questions/49512288/no-instance-of-constructor-stdthreadthread-matches-argument-list
    if (checkForExposedKing) {
        auto it = moves.begin();
        while (it != moves.end()) {
            auto asyncResult = std::async(&Board::doesMoveExposeKing, this, *it);
            bool exposesCheck = asyncResult.get();
            if (exposesCheck) {
                it = moves.erase(it);
            }
            else {
                it++;
            }
        }
    }

    return moves;
}

// Return the board "score", currently the difference between black and white piece values
// Positive value indicates black advantage
// Negative value indicates white advantage
double Board::evaluateScore() {
    double blackTotal = 0;
    double whiteTotal = 0;

    // Currently, the score of a player's board is the inverse weighted sum of their opponent's pieces
    for (auto it = _pieces.begin(); it != _pieces.end(); it++) {
        switch (it->getType()) {
        case PieceType::PAWN:
            if (it->getColor() == Color::BLACK) {
                blackTotal += 1;
            }
            else {
                whiteTotal += 1;
            }
            break;
        case PieceType::KNIGHT:
            if (it->getColor() == Color::BLACK) {
                blackTotal += 3;
            }
            else {
                whiteTotal += 3;
            }
            break;
        case PieceType::BISHOP:
            if (it->getColor() == Color::BLACK) {
                blackTotal += 3;
            }
            else {
                whiteTotal += 3;
            }
            break;
        case PieceType::ROOK:
            if (it->getColor() == Color::BLACK) {
                blackTotal += 5;
            }
            else {
                whiteTotal += 5;
            }
            break;
        case PieceType::QUEEN:
            if (it->getColor() == Color::BLACK) {
                blackTotal += 9;
            }
            else {
                whiteTotal += 9;
            }
            break;
        case PieceType::KING:
            if (it->getColor() == Color::BLACK) {
                blackTotal += 200;
            }
            else {
                whiteTotal += 200;
            }
            break;
        default:
            break;
        }
    }

    return blackTotal - whiteTotal;
}

void Board::playMove(std::string move) {
    std::string fromPos = move.substr(0, 2);
    std::string toPos = move.substr(2, 2);
    _pieces[positionIndex.at(toPos)] = _pieces[positionIndex.at(fromPos)];
    _pieces[positionIndex.at(fromPos)] = Piece(Color::NONE, PieceType::NONE);

    // If this was a promotion move, change the piece to a queen.
    // TODO: Allow promotion to any piece type
    // TODO: After promotion re-evaluate check and checkmate
    if (move[move.length() - 1] == 'p') {
        _pieces[positionIndex.at(toPos)].setType(PieceType::QUEEN);
    }
    // If this was a castling move, adjust the rook position
    else if (move[move.length() - 1] == 'c') {
        std::string rookFrom;
        std::string rookTo;
        if (move == "e1c1c") {
            rookFrom = "a1";
            rookTo = "d1";
        }
        else if (move == "e1g1c") {
            rookFrom = "h1";
            rookTo = "f1";
        }
        else if (move == "e8c8c") {
            rookFrom = "a8";
            rookTo = "d8";
        }
        else if (move == "e8g8c") {
            rookFrom = "h8";
            rookTo = "f8";
        }
        _pieces[positionIndex.at(rookTo)] = _pieces[positionIndex.at(rookFrom)];
        _pieces[positionIndex.at(rookFrom)] = Piece(Color::NONE, PieceType::NONE);
    }
}

bool Board::isInCheck(Color colorInCheck) {
    Color opponentColor = (colorInCheck == Color::WHITE) ? Color::BLACK : Color::WHITE;
    int kingIndex = -1;

    // Get the board index of the king that might in check
    // TODO: It's probably cheaper to maintain class pointers to the king pieces?
    for (auto it = _pieces.begin(); it != _pieces.end(); it++) {
        if (it->getColor() == colorInCheck && it->getType() == PieceType::KING) {
            kingIndex = it - _pieces.begin();
        }
    }

    // Now look at all attacking board positions from the king's perspective
    if (colorInCheck == Color::BLACK) {
        // Pawn attacking black king from lower left
        if (kingIndex <= 55 && (kingIndex % 8) >= 1 && _pieces[kingIndex + 7].getColor() == Color::WHITE && _pieces[kingIndex + 7].getType() == PieceType::PAWN) {
            return true;
        }
        // Pawn attacking black king from lower right
        if (kingIndex <= 54 && (kingIndex % 8) <= 6 && _pieces[kingIndex + 9].getColor() == Color::WHITE && _pieces[kingIndex + 9].getType() == PieceType::PAWN) {
            return true;
        }
    }
    else if (colorInCheck == Color::WHITE) {
        // Pawn attacking white king from upper left
        if (kingIndex >= 9 && (kingIndex % 8) >= 1 && _pieces[kingIndex - 9].getColor() == Color::BLACK && _pieces[kingIndex - 9].getType() == PieceType::PAWN) {
            return true;
        }
        // Pawn attacking black king from upper right
        if (kingIndex >= 8 && (kingIndex % 8) <= 6 && _pieces[kingIndex - 7].getColor() == Color::BLACK && _pieces[kingIndex - 7].getType() == PieceType::PAWN) {
            return true;
        }
    }
    // Look for knights
    if ((kingIndex - 17) >= 0 && (kingIndex % 8) >= 1 && _pieces[kingIndex - 17].getColor() == opponentColor && _pieces[kingIndex - 17].getType() == PieceType::KNIGHT) {
        return true;
    }
    if ((kingIndex - 15) >= 0 && (kingIndex % 8) <= 6 && _pieces[kingIndex - 15].getColor() == opponentColor && _pieces[kingIndex - 15].getType() == PieceType::KNIGHT) {
        return true;
    }
    if ((kingIndex - 10) >= 0 && (kingIndex % 8) >= 2 && _pieces[kingIndex - 10].getColor() == opponentColor && _pieces[kingIndex - 10].getType() == PieceType::KNIGHT) {
        return true;
    }
    if ((kingIndex - 6) >= 0 && (kingIndex % 8) <= 5 && _pieces[kingIndex - 6].getColor() == opponentColor && _pieces[kingIndex - 6].getType() == PieceType::KNIGHT) {
        return true;
    }
    if ((kingIndex + 6) < 64 && (kingIndex % 8) >= 2 && _pieces[kingIndex + 6].getColor() == opponentColor && _pieces[kingIndex + 6].getType() == PieceType::KNIGHT) {
        return true;
    }
    if ((kingIndex + 10) < 64 && (kingIndex% 8) <= 5 && _pieces[kingIndex + 10].getColor() == opponentColor && _pieces[kingIndex + 10].getType() == PieceType::KNIGHT) {
        return true;
    }
    if ((kingIndex + 15) < 64 && (kingIndex % 8) >= 1 && _pieces[kingIndex + 15].getColor() == opponentColor && _pieces[kingIndex + 15].getType() == PieceType::KNIGHT) {
        return true;
    }
    if ((kingIndex + 17) < 64 && (kingIndex % 8) <= 6 && _pieces[kingIndex + 17].getColor() == opponentColor && _pieces[kingIndex + 17].getType() == PieceType::KNIGHT) {
        return true;
    }
    // Look for bishops, rooks and queens
    for (int upleft = kingIndex - 9; upleft >= 0 && (upleft % 8) != 7; upleft -= 9) {
        if (_pieces[upleft].getColor() == opponentColor) {
            if (_pieces[upleft].getType() == PieceType::QUEEN || _pieces[upleft].getType() == PieceType::BISHOP) {
                return true;
            }
            else {
                break;
            }
        }
        if (_pieces[upleft].getColor() != Color::NONE) {
            break;
        }
    }
    for (int up = kingIndex - 8; up >= 0; up -= 8) {
        if (_pieces[up].getColor() == opponentColor) {
            if (_pieces[up].getType() == PieceType::QUEEN || _pieces[up].getType() == PieceType::ROOK) {
                return true;
            }
            else {
                break;
            }
        }
        if (_pieces[up].getColor() != Color::NONE) {
            break;
        }
    }
    for (int upright = kingIndex - 7; upright > 0 && (upright % 8) != 0; upright -= 7) {
        if (_pieces[upright].getColor() == opponentColor) {
            if (_pieces[upright].getType() == PieceType::QUEEN || _pieces[upright].getType() == PieceType::BISHOP) {
                return true;
            }
            else {
                break;
            }
        }
        if (_pieces[upright].getColor() != Color::NONE) {
            break;
        }
    }
    for (int left = kingIndex - 1; left >= kingIndex - (kingIndex % 8); left -= 1) {
        if (_pieces[left].getColor() == opponentColor) {
            if (_pieces[left].getType() == PieceType::QUEEN || _pieces[left].getType() == PieceType::ROOK) {
                return true;
            }
            else {
                break;
            }
        }
        if (_pieces[left].getColor() != Color::NONE) {
            break;
        }
    }
    for (int right = kingIndex + 1; right <= kingIndex + (7 - (kingIndex % 8)); right += 1) {
        if (_pieces[right].getColor() == opponentColor) {
            if (_pieces[right].getType() == PieceType::QUEEN || _pieces[right].getType() == PieceType::ROOK) {
                return true;
            }
            else {
                break;
            }
        }
        if (_pieces[right].getColor() != Color::NONE) {
            break;
        }
    }
    for (int downleft = kingIndex + 7; downleft <= 62 && (downleft % 8) != 7; downleft += 7) {
        if (_pieces[downleft].getColor() == opponentColor) {
            if (_pieces[downleft].getType() == PieceType::QUEEN || _pieces[downleft].getType() == PieceType::BISHOP) {
                return true;
            }
            else {
                break;
            }
        }
        if (_pieces[downleft].getColor() != Color::NONE) {
            break;
        }
    }
    for (int down = kingIndex + 8; down <= 63; down += 8) {
        if (_pieces[down].getColor() == opponentColor) {
            if (_pieces[down].getType() == PieceType::QUEEN || _pieces[down].getType() == PieceType::ROOK) {
                return true;
            }
            else {
                break;
            }
        }
        if (_pieces[down].getColor() != Color::NONE) {
            break;
        }
    }
    for (int downright = kingIndex + 9; downright <= 63 && (downright % 8) != 0; downright += 9) {
        if (_pieces[downright].getColor() == opponentColor) {
            if (_pieces[downright].getType() == PieceType::QUEEN || _pieces[downright].getType() == PieceType::BISHOP) {
                return true;
            }
            else {
                break;
            }
        }
        if (_pieces[downright].getColor() != Color::NONE) {
            break;
        }
    }
    // Look for kings
    if (kingIndex - 9 > 0 && (kingIndex % 8) >= 1 && _pieces[kingIndex - 9].getColor() == opponentColor && _pieces[kingIndex - 9].getType() == PieceType::KING) {
        return true;
    }
    if (kingIndex - 8 > 0 && _pieces[kingIndex - 8].getColor() == opponentColor && _pieces[kingIndex - 8].getType() == PieceType::KING) {
        return true;
    }
    if (kingIndex - 7 > 0 && (kingIndex % 8) <= 6 && _pieces[kingIndex - 6].getColor() == opponentColor && _pieces[kingIndex - 6].getType() == PieceType::KING) {
        return true;
    }
    if (kingIndex - 1 > 0 && (kingIndex % 8) >= 1 && _pieces[kingIndex - 1].getColor() == opponentColor && _pieces[kingIndex - 1].getType() == PieceType::KING) {
        return true;
    }
    if (kingIndex + 1 < 64 && (kingIndex % 8) <= 6 && _pieces[kingIndex + 1].getColor() == opponentColor && _pieces[kingIndex + 1].getType() == PieceType::KING) {
        return true;
    }
    if (kingIndex + 7 < 64 && (kingIndex % 8) >= 1 && _pieces[kingIndex + 7].getColor() == opponentColor && _pieces[kingIndex + 7].getType() == PieceType::KING) {
        return true;
    }
    if (kingIndex + 8 < 64 && _pieces[kingIndex + 8].getColor() == opponentColor && _pieces[kingIndex + 8].getType() == PieceType::KING) {
        return true;
    }
    if (kingIndex + 9 < 64 && (kingIndex % 8) <= 6 && _pieces[kingIndex + 9].getColor() == opponentColor && _pieces[kingIndex + 9].getType() == PieceType::KING) {
        return true;
    }


    // If we got this far, colorInCheck's king is not in check. Return true
    return false;
}

bool Board::doesMoveExposeKing(std::string move) {
    
    bool isCheck = false;
    std::string fromPos = move.substr(0, 2);
    std::string toPos = move.substr(2, 2);
    Piece movePiece = _pieces[positionIndex.at(fromPos)];
    Piece capturedPiece = _pieces[positionIndex.at(toPos)];
    Color moveColor = movePiece.getColor();
    
    // Temporarily play the move
    // TODO: This is probably not thread safe. Either copy the board or add a lock.
    _pieces[positionIndex.at(fromPos)] = Piece(Color::NONE, PieceType::NONE);
    _pieces[positionIndex.at(toPos)] = movePiece;

    // Look for check
    isCheck = isInCheck(moveColor);

    // Now undo the move
    _pieces[positionIndex.at(fromPos)] = movePiece;
    _pieces[positionIndex.at(toPos)] = capturedPiece;

    return isCheck;
}

bool Board::doesMoveCheckOpponent(std::string move) {

    bool isCheck = false;
    std::string fromPos = move.substr(0, 2);
    std::string toPos = move.substr(2, 2);
    Piece movePiece = _pieces[positionIndex.at(fromPos)];
    Piece capturedPiece = _pieces[positionIndex.at(toPos)];
    Color opponentColor = movePiece.getColor() == Color::WHITE ? Color::BLACK : Color::WHITE;

    // Temporarily play the move
    // TODO: This is probably not thread safe. Either copy the board or add a lock.
    _pieces[positionIndex.at(fromPos)] = Piece(Color::NONE, PieceType::NONE);
    _pieces[positionIndex.at(toPos)] = movePiece;

    // Look for check
    isCheck = isInCheck(opponentColor);

    // Now undo the move
    _pieces[positionIndex.at(fromPos)] = movePiece;
    _pieces[positionIndex.at(toPos)] = capturedPiece;

    return isCheck;
}

bool Board::doesMoveCheckmateOpponent(std::string move) {

    bool isCheckmate = false;
    std::string fromPos = move.substr(0, 2);
    std::string toPos = move.substr(2, 2);
    Piece movePiece = _pieces[positionIndex.at(fromPos)];
    Piece capturedPiece = _pieces[positionIndex.at(toPos)];
    Color opponentColor = movePiece.getColor() == Color::WHITE ? Color::BLACK : Color::WHITE;

    // Temporarily play the move
    // TODO: This is probably not thread safe. Either copy the board or add a lock.
    _pieces[positionIndex.at(fromPos)] = Piece(Color::NONE, PieceType::NONE);
    _pieces[positionIndex.at(toPos)] = movePiece;

    // Count the opponent's available moves. 
    std::vector<std::string> opponentMoves = getColorAvailableMoves(opponentColor);
    if (opponentMoves.size() == 0) {
        isCheckmate = true;
    }

    // Now undo the move
    _pieces[positionIndex.at(fromPos)] = movePiece;
    _pieces[positionIndex.at(toPos)] = capturedPiece;

    return isCheckmate;
}