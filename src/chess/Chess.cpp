#include "../../include/chess/Chess.hpp"
#include "../../include/chess/board/Board.hpp"
#include "../../include/chess/piece/Piece.hpp"
#include "../../include/chess/algorithms/Algorithm.hpp"
#include <vector> // std::vector
#include <unordered_set> // std::unordered_set
#include <iostream>
#include <memory> // std::unique_ptr

using namespace std;

Chess::Chess(bool blank) {
    whitePieces = unordered_set<Piece*>();
    blackPieces = unordered_set<Piece*>();
    board = new Board(this, blank);
}

Chess::Chess(Chess* chess) {
    whitePieces = unordered_set<Piece*>();
    blackPieces = unordered_set<Piece*>();
    setBoard(chess->getBoard()->clone(this));
}

Chess* Chess::clone() {
    Chess* chess = new Chess(this);
    return chess;
}

Chess::~Chess() {
    delete board;
}

Chess* Chess::blankChess() {
    Chess* chess = new Chess(true);
    return chess;
}

Board* Chess::getBoard() {
    return board;
}

unordered_set<Piece*> Chess::getWhitePieces() {
    return whitePieces;
}

unordered_set<Piece*> Chess::getBlackPieces() {
    return blackPieces;
}

void Chess::addWhitePiece(Piece* piece) {
    whitePieces.insert(piece);
}

void Chess::addBlackPiece(Piece* piece) {
    blackPieces.insert(piece);
}

bool Chess::isCheck(bool isWhite) {
    unordered_set<Piece*> pieces = isWhite ? blackPieces : whitePieces;
    for (Piece* piece : pieces) {
        unordered_set<pair<int,int>, pair_hash> moves = piece->getPossibleMoves(board);
        for (pair<int,int> move : moves) {
            Piece* targetPiece = board->getPiece(move.first, move.second);
            if (targetPiece != nullptr && targetPiece->isKing()){
                return true;
            }
        }
    }
    return false;
}

bool Chess::isStalemate(bool isWhite) {
    for (Piece* piece : isWhite ? whitePieces : blackPieces) {
        unordered_set<pair<int,int>, pair_hash> moves = piece->getPossibleMoves(board);
        if (moves.size() > 0) {
            for (pair<int,int> move : moves) {
                // Réinitialiser l'objet Chess à chaque itération
                std::unique_ptr<Chess> cloneChess(new Chess(this));
                cloneChess->movePiece(piece->getPosition().first, piece->getPosition().second, move.first, move.second);
                if (!cloneChess->isCheck(isWhite)) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Chess::isCheckmate(bool isWhite) {
    if (!isCheck(isWhite)) {
        return false;
    }
    for (Piece* piece : isWhite ? whitePieces : blackPieces) {
        pair<int,int> basePos = piece->getPosition();
        unordered_set<pair<int,int>, pair_hash> moves = piece->getPossibleMoves(board);
        for (pair<int,int> move : moves) {
            // Réinitialiser l'objet Chess à chaque itération
            std::unique_ptr<Chess> cloneChess(new Chess(this));
            cloneChess->movePiece(basePos.first, basePos.second, move.first, move.second);
            if (!cloneChess->isCheck(isWhite)) {
                return false;
            }
        }   
    }
    return true;
}



void Chess::setBoard(Board* board) {
    if (this->board != nullptr) {
        delete this->board;
    }
    this->board = board;
}

Piece* Chess::switchPiece(Piece* piece, char symbol) {
    if (piece == nullptr || !piece->isSwitchable()) {
        return piece;
    }
    bool isWhite = piece->isWhite();
    Piece* newPiece = nullptr;
    if (symbol == 'Q') {
        newPiece = new Queen(isWhite);
    } else if (symbol == 'B') {
        newPiece = new Bishop(isWhite);
    } else if (symbol == 'N') {
        newPiece = new Knight(isWhite);
    } else {
        newPiece = new Rook(isWhite);
    } 
    board->setPiece(piece->getPosition().first, piece->getPosition().second, newPiece);
    deletePiece(piece);
    addPiece(newPiece);
    return newPiece;
}

void Chess::movePiece(int i, int j, int x, int y) {
    if (board->getPiece(i,j) == nullptr) {
        return;
    }
    board->movePiece(i, j, x, y);
}

void Chess::deletePiece(Piece* piece) {
    if (piece == nullptr) {
        return;
    }
    unordered_set<Piece*>* pieces = piece->isWhite() ? &whitePieces : &blackPieces;
    pieces->erase(piece);
    delete piece;
}

void Chess::addPiece(Piece* piece) {
    if (piece == nullptr) {
        return;
    }
    unordered_set<Piece*>* pieces = piece->isWhite() ? &whitePieces : &blackPieces;
    pieces->insert(piece);
}

bool Chess::canCastle(bool isWhite, bool isKingSide) {
    unordered_set<Piece*>* pieces = isWhite ? &blackPieces : &whitePieces;
    int row = isWhite ? 0 : 7;
    int kingColumn = 4;
    int rookColumn = isKingSide ? 7 : 0;
    if (board->getPiece(row, kingColumn) == nullptr || board->getPiece(row, kingColumn)->getSymbol() != (isWhite ? 'K' : 'k')) {
        return false;
    }
    if (board->getPiece(row, rookColumn) == nullptr || board->getPiece(row, rookColumn)->getSymbol() != (isWhite ? 'R' : 'r')) {
        return false;
    }
    if (board->getPiece(row, kingColumn)->hasMoved() || board->getPiece(row, rookColumn)->hasMoved()) {
        return false;
    }
    // board->display();
    for (int i = 1; i <= (isKingSide ? 2 : 3); i++) {
        if (board->getPiece(row, kingColumn + i * (isKingSide ? 1 : -1)) != nullptr) {
            return false;
        }
    }
    for (Piece* piece : *pieces) {
        unordered_set<pair<int,int>, pair_hash> moves = piece->getPossibleMoves(board);
        for (pair<int,int> move : moves) {
                if (move.first == row) {
                    for (int i = 1; i < (isKingSide ? 2 : 3); i++) {
                        if (move.second == kingColumn + i * (isKingSide ? 1 : -1)) {
                            return false;
                    }
                }
            }
        }
    }
    return true;
}

void Chess::makeCastle(bool isWhite, bool isKingSide) {
    int row = isWhite ? 0 : 7;
    int kingColumn = 4;
    int rookColumn = isKingSide ? 7 : 0;
    board->movePiece(row, kingColumn, row, kingColumn + (isKingSide ? 2 : -2));
    board->movePiece(row, rookColumn, row, kingColumn + (isKingSide ? 1 : -1));
}

void Chess::setWhitePieces(unordered_set<Piece*> whitePieces) {
    if (this->whitePieces.size() > 0) {
        for (Piece* piece : this->whitePieces) {
            delete piece;
        }
    }
    for (Piece* piece : whitePieces) {
        addWhitePiece(piece->clone());
    }
}

void Chess::setBlackPieces(unordered_set<Piece*> blackPieces) {
    if (this->blackPieces.size() > 0) {
        for (Piece* piece : this->blackPieces) {
            delete piece;
        }
    }
    for (Piece* piece : blackPieces) {
        addBlackPiece(piece->clone());
    }
}

void Chess::setPiece(int x, int y, Piece* piece) {
    board->setPiece(x, y, piece);
}

// play

void Chess::play(Algorithm* algorithm1, Algorithm* algorithm2) {
    bool isWhite = true;
    while (!isCheckmate(isWhite) && !isStalemate(isWhite)) {
        cout << (isWhite ? "White" : "Black") << " to play" << endl;
        pair<Piece*,pair<int,int>> move = isWhite ? algorithm1->getBestMove(this, isWhite) : algorithm2->getBestMove(this, isWhite);
        cout << "Move found" << endl;
        movePiece(move.first->getPosition().first, move.first->getPosition().second, move.second.first, move.second.second);
        board->display();
        isWhite = !isWhite;
    }
}


