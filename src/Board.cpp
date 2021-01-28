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

Board::Board() {

}

Board::Board(json &positions) {
    setPieces(positions);
}

Board::~Board() {

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

        PieceColor color{};
        if (pieceClass.find("black") != std::string::npos) {
            color = PieceColor::BLACK;
        }
        if (pieceClass.find("white") != std::string::npos) {
            color = PieceColor::WHITE;
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

std::vector<std::string> Board::getPieceAvailableMoves(std::string position) {
    int index = positionIndex.at(position);
    Piece piece = _pieces[index];
    PieceColor opponentColor = (piece.getColor() == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
    std::vector<std::string> moves{};

    // Pawns need movement rules specific to each color (since they move in opposite directions)
    if (piece.getColor() == PieceColor::WHITE && piece.getType() == PieceType::PAWN) {
        if (index >= 8 && _pieces[index - 8].getType() == PieceType::NONE) {
            moves.push_back(position + indexPosition.at(index - 8));
        }
        if (index >= 48 && index <= 55 && _pieces[index - 16].getType() == PieceType::NONE) {
            moves.push_back(position + indexPosition.at(index - 16));
        }
        if (index >= 8 && _pieces[index - 7].getColor() == opponentColor) {
            moves.push_back(position + indexPosition.at(index - 7));
        }
        if (index >= 9 && _pieces[index - 9].getColor() == opponentColor) {
            moves.push_back(position + indexPosition.at(index - 9));
        }
    }
    else if (piece.getColor() == PieceColor::BLACK && piece.getType() == PieceType::PAWN) {
        if (index <= 55 && _pieces[index + 8].getType() == PieceType::NONE) {
            moves.push_back(position + indexPosition.at(index + 8));
        }
        if (index >= 8 && index <= 15 && _pieces[index + 16].getType() == PieceType::NONE) {
            moves.push_back(position + indexPosition.at(index + 16));
        }
        if (index <= 55 && _pieces[index + 7].getColor() == opponentColor) {
            moves.push_back(position + indexPosition.at(index + 7));
        }
        if (index <= 54 && _pieces[index + 9].getColor() == opponentColor) {
            moves.push_back(position + indexPosition.at(index + 9));
        }
    }
    // All other pieces have the same movement rules, regardless of color
    else if (piece.getType() == PieceType::KNIGHT) {
        if ((index - 17) >= 0 && index >= ((index / 8) * 8) + 1 && _pieces[index - 17].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index - 17));
        }
        if ((index - 15) >= 0 && index < (((index / 8) + 1) * 8) - 1 && _pieces[index - 15].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index - 15));
        }
        if ((index - 10) >= 0 && index >= ((index / 8) * 8) + 2 && _pieces[index - 10].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index - 10));
        }
        if ((index - 6) >= 0 && index < (((index / 8) + 1) * 8) - 2 && _pieces[index - 6].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index - 6));
        }
        if ((index + 6) < 64 && index >= ((index / 8) * 8) + 2 && _pieces[index + 6].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index + 6));
        }
        if ((index + 10) < 64 && index < (((index / 8) + 1) * 8) - 2 && _pieces[index + 10].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index + 10));
        }
        if ((index + 15) < 64 && index >= ((index / 8) * 8) + 1 && _pieces[index + 15].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index + 15));
        }
        if ((index + 17) < 64 && index < (((index / 8) + 1) * 8) - 1 && _pieces[index + 17].getColor() != piece.getColor()) {
            moves.push_back(position + indexPosition.at(index + 17));
        }
    }
    else if (piece.getType() == PieceType::ROOK) {
        for (int up = index - 8; up > 0; up -= 8) {
            if (_pieces[up].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(up));
            }
            if (_pieces[up].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int down = index + 8; down < 64; down += 8) {
            if (_pieces[down].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(down));
            }
            if (_pieces[down].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int left = index - 1; left >= 0 && left >= index - (index % 8); left -= 1) {
            if (_pieces[left].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(left));
            }
            if (_pieces[left].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int right = index + 1; right <= 63 && right <= index + (7 - (index % 8)); right += 1) {
            if (_pieces[right].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(right));
            }
            if (_pieces[right].getColor() != PieceColor::NONE) {
                break;
            }
        }
    }
    else if (piece.getType() == PieceType::BISHOP) {
        for (int upleft = index - 9; upleft > 0 && (upleft % 8) != 7; upleft -= 9) {
            if (_pieces[upleft].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(upleft));
            }
            if (_pieces[upleft].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int upright = index - 7; upright > 0 && (upright % 8) != 0; upright -= 7) {
            if (_pieces[upright].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(upright));
            }
            if (_pieces[upright].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int downleft = index + 7; downleft < 64 && (downleft % 8) != 7; downleft += 7) {
            if (_pieces[downleft].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(downleft));
            }
            if (_pieces[downleft].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int downright = index + 9; downright < 64 && (downright % 8) != 0; downright += 9) {
            if (_pieces[downright].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(downright));
            }
            if (_pieces[downright].getColor() != PieceColor::NONE) {
                break;
            }
        }
    }
    else if (piece.getType() == PieceType::QUEEN) {
        for (int upleft = index - 9; upleft > 0 && (upleft % 8) != 7; upleft -= 9) {
            if (_pieces[upleft].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(upleft));
            }
            if (_pieces[upleft].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int up = index - 8; up > 0; up -= 8) {
            if (_pieces[up].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(up));
            }
            if (_pieces[up].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int upright = index - 7; upright > 0 && (upright % 8) != 0; upright -= 7) {
            if (_pieces[upright].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(upright));
            }
            if (_pieces[upright].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int left = index - 1; left >= index - (index % 8); left -= 1) {
            if (_pieces[left].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(left));
            }
            if (_pieces[left].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int right = index + 1; right <= index + (7 - (index % 8)); right += 1) {
            if (_pieces[right].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(right));
            }
            if (_pieces[right].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int downleft = index + 7; downleft < 64 && (downleft % 8) != 7; downleft += 7) {
            if (_pieces[downleft].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(downleft));
            }
            if (_pieces[downleft].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int down = index + 8; down < 64; down += 8) {
            if (_pieces[down].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(down));
            }
            if (_pieces[down].getColor() != PieceColor::NONE) {
                break;
            }
        }
        for (int downright = index + 9; downright < 64 && (downright % 8) != 0; downright += 9) {
            if (_pieces[downright].getColor() != piece.getColor()) {
                moves.push_back(position + indexPosition.at(downright));
            }
            if (_pieces[downright].getColor() != PieceColor::NONE) {
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
        if (piece.getColor() == PieceColor::BLACK && index == 4) {
            if (_pieces[0].getColor() == PieceColor::BLACK && _pieces[0].getType() == PieceType::ROOK
                    && _pieces[1].getType() == PieceType::NONE && _pieces[2].getType() == PieceType::NONE
                    && _pieces[3].getType() == PieceType::NONE) {
                moves.push_back(position + indexPosition.at(2) + "c");
            }
            else if (_pieces[7].getColor() == PieceColor::BLACK && _pieces[7].getType() == PieceType::ROOK
                    && _pieces[5].getType() == PieceType::NONE && _pieces[6].getType() == PieceType::NONE) {
                moves.push_back(position + indexPosition.at(6) + "c");
            }
        }
        else if (piece.getColor() == PieceColor::WHITE && index == 60) {
            if (_pieces[56].getColor() == PieceColor::WHITE && _pieces[56].getType() == PieceType::ROOK
                    && _pieces[57].getType() == PieceType::NONE && _pieces[58].getType() == PieceType::NONE
                    && _pieces[59].getType() == PieceType::NONE) {
                moves.push_back(position + indexPosition.at(58) + "c");
            }
            else if (_pieces[63].getColor() == PieceColor::WHITE && _pieces[63].getType() == PieceType::ROOK
                    && _pieces[61].getType() == PieceType::NONE && _pieces[62].getType() == PieceType::NONE) {
                moves.push_back(position + indexPosition.at(62) + "c");
            }
        }
    }

    // Lastly, make sure this move does not put our king in check! If it does, remove from the list
    // Now launch the check function as a thread
    // Syntax for creating a new thread:
    // https://thispointer.com/c11-start-thread-by-member-function-with-arguments/
    // https://stackoverflow.com/questions/49512288/no-instance-of-constructor-stdthreadthread-matches-argument-list
    // 

    auto it = moves.begin();
    while (it != moves.end()) {
        
        /* These examples launch a std::thread but cannot return a value
        std::string thisMove = *it;
        std::thread testThread([this, thisMove] { this->doesMoveExposeCheck(thisMove); });
        std::thread testThread(&Board::doesMoveExposeCheck, this, thisMove);
        if (doesMoveExposeCheck(*it)) {
            it = moves.erase(it);
        }
        else {
            it++;
        }
        testThread.join();
        */

        auto asyncResult = std::async(&Board::doesMoveExposeCheck, this, *it);
        bool exposesCheck = asyncResult.get();
        if (exposesCheck) {
            it = moves.erase(it);
        }
        else {
            it++;
        }
    }

    return moves;
}

std::string Board::getMove(PieceColor color) {
    std::vector<std::string> allMoves{};

    // Setup a collection of all possible moves for the specified color
    for (auto it = _pieces.begin(); it != _pieces.end(); it++) {
        if (it->getColor() == color) {
            std::string position = indexPosition.at(it - _pieces.begin());
            std::vector<std::string> thisPosMoves = getPieceAvailableMoves(position);
            for (auto& move : thisPosMoves) {
                allMoves.push_back(move);
            }
        }
    }

    // Seed a random number generator using current time milliseconds
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::srand(millis);

    // Now pick a move at random
    auto it = allMoves.begin();
    std::advance(it, std::rand() % allMoves.size());
    std::string move = *it;

    return move;
}

void Board::playMove(std::string move) {
    std::string fromPos = move.substr(0, 2);
    std::string toPos = move.substr(2, 2);
    _pieces[positionIndex.at(toPos)] = _pieces[positionIndex.at(fromPos)];
    _pieces[positionIndex.at(fromPos)] = Piece(PieceColor::NONE, PieceType::NONE);
}

bool Board::isInCheck(PieceColor checkColor) {
    PieceColor opponentColor = (checkColor == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
    int kingIndex = -1;

    // Get the board index of the king that might in check
    // TODO: It's probably cheaper to maintain class pointers to the king pieces?
    for (auto it = _pieces.begin(); it != _pieces.end(); it++) {
        if (it->getColor() == checkColor && it->getType() == PieceType::KING) {
            kingIndex = it - _pieces.begin();
        }
    }

    // Now look at all attacking board positions from the king's perspective
    if (checkColor == PieceColor::BLACK) {
        // Pawn attacking black king from lower left
        if (kingIndex % 8 >= 1 && _pieces[kingIndex + 7].getColor() == PieceColor::WHITE && _pieces[kingIndex + 7].getType() == PieceType::PAWN) {
            return true;
        }
        // Pawn attacking black king from lower right
        if (kingIndex % 8 <= 6 && _pieces[kingIndex + 9].getColor() == PieceColor::WHITE && _pieces[kingIndex + 9].getType() == PieceType::PAWN) {
            return true;
        }
    }
    else if (checkColor == PieceColor::WHITE) {
        // Pawn attacking white king from upper left
        if (kingIndex % 8 >= 1 && _pieces[kingIndex - 9].getColor() == PieceColor::BLACK && _pieces[kingIndex - 9].getType() == PieceType::PAWN) {
            return true;
        }
        // Pawn attacking black king from upper right
        if (kingIndex % 8 <= 6 && _pieces[kingIndex - 7].getColor() == PieceColor::BLACK && _pieces[kingIndex - 9].getType() == PieceType::PAWN) {
            return true;
        }
    }
    // Do some crazy sorcery to look for knights, deal with this later
    // Check open spaces along each axis, see if we run into a piece that can capture the king
    for (int upleft = kingIndex - 9; upleft > 0 && (upleft % 8) != 7; upleft -= 9) {
        if (_pieces[upleft].getColor() == opponentColor) {
            if (_pieces[upleft].getType() == PieceType::QUEEN || _pieces[upleft].getType() == PieceType::BISHOP) {
                return true;
            }
            else {
                break;
            }
        }
        if (_pieces[upleft].getColor() != PieceColor::NONE) {
            break;
        }
    }
    for (int up = kingIndex - 8; up > 0; up -= 8) {
        if (_pieces[up].getColor() == opponentColor) {
            if (_pieces[up].getType() == PieceType::QUEEN || _pieces[up].getType() == PieceType::ROOK) {
                return true;
            }
            else {
                break;
            }
        }
        if (_pieces[up].getColor() != PieceColor::NONE) {
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
        if (_pieces[upright].getColor() != PieceColor::NONE) {
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
        if (_pieces[left].getColor() != PieceColor::NONE) {
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
        if (_pieces[right].getColor() != PieceColor::NONE) {
            break;
        }
    }
    for (int downleft = kingIndex + 7; downleft < 64 && (downleft % 8) != 7; downleft += 7) {
        if (_pieces[downleft].getColor() == opponentColor) {
            if (_pieces[downleft].getType() == PieceType::QUEEN || _pieces[downleft].getType() == PieceType::BISHOP) {
                return true;
            }
            else {
                break;
            }
        }
        if (_pieces[downleft].getColor() != PieceColor::NONE) {
            break;
        }
    }
    for (int down = kingIndex + 8; down < 64; down += 8) {
        if (_pieces[down].getColor() == opponentColor) {
            if (_pieces[down].getType() == PieceType::QUEEN || _pieces[down].getType() == PieceType::ROOK) {
                return true;
            }
            else {
                break;
            }
        }
        if (_pieces[down].getColor() != PieceColor::NONE) {
            break;
        }
    }
    for (int downright = kingIndex + 9; downright < 64 && (downright % 8) != 0; downright += 9) {
        if (_pieces[downright].getColor() == opponentColor) {
            if (_pieces[downright].getType() == PieceType::QUEEN || _pieces[downright].getType() == PieceType::BISHOP) {
                return true;
            }
            else {
                break;
            }
        }
        if (_pieces[downright].getColor() != PieceColor::NONE) {
            break;
        }
    }


    // If we got this far, checkColor's king is not in check. Return true
    return false;
}

bool Board::doesMoveExposeCheck(std::string move) {
    
    bool isCheck = false;
    std::string fromPos = move.substr(0, 2);
    std::string toPos = move.substr(2, 4);
    Piece movePiece = _pieces[positionIndex.at(fromPos)];
    Piece capturedPiece = _pieces[positionIndex.at(toPos)];
    PieceColor moveColor = movePiece.getColor();
    
    // Temporarily play the move
    // TODO: This is almost certainly not thread safe. Either copy the board or add a lock.
    _pieces[positionIndex.at(fromPos)] = Piece(PieceColor::NONE, PieceType::NONE);
    _pieces[positionIndex.at(toPos)] = movePiece;

    // Look for check
    isCheck = isInCheck(moveColor);

    // Now undo the move
    _pieces[positionIndex.at(fromPos)] = movePiece;
    _pieces[positionIndex.at(toPos)] = capturedPiece;

    return isCheck;
}