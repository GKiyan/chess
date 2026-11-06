#include "../../include/chess/Chess.hpp"
#include "../../include/chess/piece/Piece.hpp"
#include "../../include/chess/board/Board.hpp"
#include <cassert>
#include <iostream>

using namespace std;

void test_BlankChess() {
    Chess* chess = new Chess(true);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            assert(chess->getBoard()->getPiece(i, j) == nullptr);
        }
    }

    assert(chess->getBlackPieces().size() == 0);
    assert(chess->getWhitePieces().size() == 0);

    delete chess;
}

void test_clone() {
    Chess* chess = new Chess(true);
    Chess* clone = new Chess(chess);

    clone->setPiece(0, 0, new King(true));
    chess->setPiece(0, 0, new King(false));

    assert(chess->getBoard() != clone->getBoard());
    assert(chess->getWhitePieces() != clone->getWhitePieces());
    assert(chess->getBlackPieces() != clone->getBlackPieces());

    delete chess;
    delete clone;
}

void test_isCheck() {
    Chess* chess = new Chess(true);

    chess->setPiece(4, 4, new King(true));
    chess->setPiece(2, 4, new Rook(false));

    assert(chess->isCheck(true));
    assert(!chess->isCheck(false));

    delete chess;
}

void test_isCheckmate() {
    Chess* chess = Chess::blankChess();

    chess->getBoard()->setPiece(7, 3, new King(false));
    chess->getBoard()->setPiece(6, 3, new Queen(true));
    chess->getBoard()->setPiece(4, 5, new Bishop(true));
    chess->getBoard()->setPiece(0, 1, new King(true));

    assert(chess->isCheckmate(true) == false);
    assert(chess->isCheckmate(false) == true);

    delete chess;
}

void test_stalemate() {
    Chess* chess = Chess::blankChess();

    chess->getBoard()->setPiece(0, 0, new King(true));
    chess->getBoard()->setPiece(7, 7, new King(false));
    chess->getBoard()->setPiece(6, 5, new Queen(true));

    assert(!chess->isStalemate(true));
    assert(chess->isStalemate(false));

    delete chess;
}

void test_canCastle() {
    Chess* chess = Chess::blankChess();

    assert(!chess->canCastle(true, false));
    assert(!chess->canCastle(true, true));

    chess->getBoard()->setPiece(0, 4, new King(true));
    chess->getBoard()->setPiece(0, 0, new Rook(true));

    assert(chess->canCastle(true, false));
    
    delete chess;

    chess = Chess::blankChess();

    chess->getBoard()->setPiece(0, 4, new King(true));
    chess->getBoard()->setPiece(0, 7, new Rook(true));

    assert(chess->canCastle(true, true));

    delete chess;
}

void test_makeCastle() {
    Chess* chess = Chess::blankChess();

    chess->getBoard()->setPiece(0, 4, new King(true));
    chess->getBoard()->setPiece(0, 0, new Rook(true));

    chess->makeCastle(true, false);

    assert(chess->getBoard()->getPiece(0, 2)->isKing());
    assert(chess->getBoard()->getPiece(0, 3)->isRook());

    delete chess;

    chess = Chess::blankChess();

    chess->getBoard()->setPiece(0, 4, new King(true));
    chess->getBoard()->setPiece(0, 7, new Rook(true));

    chess->makeCastle(true, true);

    assert(chess->getBoard()->getPiece(0, 6)->isKing());
    assert(chess->getBoard()->getPiece(0, 5)->isRook());

    delete chess;
}

void test_switchPiece() {
    Chess* chess = Chess::blankChess();
    Piece* piece = new Queen(true);

    chess->setPiece(0, 0, piece);
    chess->switchPiece(piece, 'Q');

    assert(chess->getBoard()->getPiece(0, 0)->isQueen());

    delete chess;
}

int main() {
    std::cout << "Blank chess tests" << std::endl;
    test_BlankChess();

    std::cout << "Check tests" << std::endl;
    test_isCheck();

    std::cout << "Clone tests" << std::endl;
    test_clone();

    std::cout << "Checkmate tests" << std::endl;
    test_isCheckmate();

    std::cout << "Stalemate tests" << std::endl;
    test_stalemate();

    std::cout << "Can castle tests" << std::endl;
    test_canCastle();

    std::cout << "Make castle tests" << std::endl;
    test_makeCastle();

    std::cout << "Switch piece tests" << std::endl;
    test_switchPiece();
    
    std::cout << "All tests passed!" << std::endl;

    return 0;
}