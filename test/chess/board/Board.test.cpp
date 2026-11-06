#include "../../../include/chess/Chess.hpp"
#include "../../../include/chess/piece/Piece.hpp"
#include "../../../include/chess/board/Board.hpp"
#include <cassert>
#include <iostream>

using namespace std;

void test_blankBoard() {
    Chess* chess = new Chess(true);
    Board* board = Board::blankBoard(chess);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            assert(board->getPiece(i, j) == nullptr);
        }
    }

    delete board;
    delete chess;
}

void test_getPiece() {
    Chess* chess = new Chess();
    Board* board = chess->getBoard();

    assert(board->getPiece(0, 0) != nullptr);
    assert(board->getPiece(0, 0)->isRook());
    assert(board->getPiece(0, 0)->isWhite());

    assert(board->getPiece(0, 1) != nullptr);
    assert(board->getPiece(0, 1)->isKnight());
    assert(board->getPiece(0, 1)->isWhite());

    assert(board->getPiece(0, 2) != nullptr);
    assert(board->getPiece(0, 2)->isBishop());
    assert(board->getPiece(0, 2)->isWhite());

    assert(board->getPiece(0, 3) != nullptr);
    assert(board->getPiece(0, 3)->isQueen());
    assert(board->getPiece(0, 3)->isWhite());

    assert(board->getPiece(0, 4) != nullptr);
    assert(board->getPiece(0, 4)->isKing());
    assert(board->getPiece(0, 4)->isWhite());

    assert(board->getPiece(0, 5) != nullptr);
    assert(board->getPiece(0, 5)->isBishop());
    assert(board->getPiece(0, 5)->isWhite());

    assert(board->getPiece(0, 6) != nullptr);
    assert(board->getPiece(0, 6)->isKnight());
    assert(board->getPiece(0, 6)->isWhite());

    assert(board->getPiece(0, 7) != nullptr);
    assert(board->getPiece(0, 7)->isRook());
    assert(board->getPiece(0, 7)->isWhite());

    for (int i = 0; i < 8; i++) {
        assert(board->getPiece(1, i) != nullptr);
        assert(board->getPiece(1, i)->isPawn());
        assert(board->getPiece(1, i)->isWhite());
    }

    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            assert(board->getPiece(i, j) == nullptr);
        }
    }

    delete chess;
}

void test_movePiece() {
    Chess* chess = new Chess();
    Board* board = chess->getBoard();

    chess->movePiece(1, 0, 3, 0);
    assert(board->getPiece(3, 0) != nullptr);
    assert(board->getPiece(3, 0)->isPawn());
    assert(board->getPiece(3, 0)->isWhite());

    chess->movePiece(6, 0, 4, 0);
    assert(board->getPiece(4, 0) != nullptr);
    assert(board->getPiece(4, 0)->isPawn());
    assert(!board->getPiece(4, 0)->isWhite());

    chess->movePiece(0, 1, 2, 0);
    assert(board->getPiece(2, 0) != nullptr);
    assert(board->getPiece(2, 0)->isKnight());
    assert(board->getPiece(2, 0)->isWhite());

    chess->movePiece(7, 1, 5, 0);
    assert(board->getPiece(5, 0) != nullptr);
    assert(board->getPiece(5, 0)->isKnight());
    assert(!board->getPiece(5, 0)->isWhite());

    delete chess;
}

void test_display() {
    Chess* chess = new Chess();
    Board* board = chess->getBoard();

    board->display();

    delete chess;
}

void test_clone() {
    Chess* chess = new Chess();
    Chess* chessClone = new Chess(chess);

    chessClone->getBoard()->movePiece(1, 0, 2, 0);
    chess->getBoard()->movePiece(1, 0, 3, 0);

    assert(chess->getBoard() != chessClone->getBoard());
    assert(chess->getWhitePieces() != chessClone->getWhitePieces());
    assert(chess->getBlackPieces() != chessClone->getBlackPieces());

    delete chess;
    delete chessClone;
}

int main() {
    test_blankBoard();
    test_getPiece();
    test_movePiece();
    test_display();
    test_clone();

    cout << "All tests passed!" << endl;

    return 0;
}