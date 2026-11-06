#include "../../../include/chess/board/Board.hpp"
#include "../../../include/chess/piece/Piece.hpp"
#include <iostream> // std::cout, std::endl
#include <stdexcept> // std::invalid_argument
#include <vector> // std::vector

using namespace std;

Board::Board(Chess* chess, bool blank) {
    // Initialisation de l'échiquier
    this->chess = chess;
    board = vector<vector<Piece*>>(SIZE, vector<Piece*>(SIZE, nullptr));

    if (!blank) {
        // Initialisation des pièces blanches
        Piece* piece = new Queen(true);
        board[0][3] = piece;
        piece->setPosition(0, 3);
        chess->addWhitePiece(piece);

        piece = new King(true);
        board[0][4] = piece;
        piece->setPosition(0, 4);
        chess->addWhitePiece(piece);

        piece = new Bishop(true);
        board[0][2] = piece;
        piece->setPosition(0, 2);
        chess->addWhitePiece(piece);

        piece = new Bishop(true);
        board[0][5] = piece;
        piece->setPosition(0, 5);
        chess->addWhitePiece(piece);

        piece = new Knight(true);
        board[0][1] = piece;
        piece->setPosition(0, 1);
        chess->addWhitePiece(piece);

        piece = new Knight(true);
        board[0][6] = piece;
        piece->setPosition(0, 6);
        chess->addWhitePiece(piece);

        piece = new Rook(true);
        board[0][0] = piece;
        piece->setPosition(0, 0);
        chess->addWhitePiece(piece);

        piece = new Rook(true);
        board[0][7] = piece;
        piece->setPosition(0, 7);
        chess->addWhitePiece(piece);

        for (int i = 0; i < SIZE; i++) {
            piece = new Pawn(true);
            board[1][i] = piece;
            piece->setPosition(1, i);
            chess->addWhitePiece(piece);
        }

        // Initialisation des pièces noires
        piece = new Queen(false);
        board[7][3] = piece;
        piece->setPosition(7, 3);
        chess->addBlackPiece(piece);

        piece = new King(false);
        board[7][4] = piece;
        piece->setPosition(7, 4);
        chess->addBlackPiece(piece);

        piece = new Bishop(false);
        board[7][2] = piece;
        piece->setPosition(7, 2);
        chess->addBlackPiece(piece);
        
        piece = new Bishop(false);
        board[7][5] = piece;
        piece->setPosition(7, 5);
        chess->addBlackPiece(piece);

        piece = new Knight(false);
        board[7][1] = piece;
        piece->setPosition(7, 1);
        chess->addBlackPiece(piece);

        piece = new Knight(false);
        board[7][6] = piece;
        piece->setPosition(7, 6);
        chess->addBlackPiece(piece);

        piece = new Rook(false);
        board[7][0] = piece;
        piece->setPosition(7, 0);
        chess->addBlackPiece(piece);

        piece = new Rook(false);
        board[7][7] = piece;
        piece->setPosition(7, 7);
        chess->addBlackPiece(piece);

        for (int i = 0; i < SIZE; i++) {
            piece = new Pawn(false);
            board[6][i] = piece;
            piece->setPosition(6, i);
            chess->addBlackPiece(piece);
        }
    }
}

Board::~Board() {
    // Libération de la mémoire allouée pour les pièces
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] != nullptr) {
                delete board[i][j];
            }
        }
    }
}

Board* Board::blankBoard(Chess* chess) {
    Board* board = new Board(chess, true);
    return board;
}

Piece* Board::getPiece(int x, int y) {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
        throw invalid_argument(to_string(x) + ", " + to_string(y) + " is out of bounds");
    }
    return board[x][y];
}

Chess* Board::getChess() {
    return chess;
}

void Board::movePiece(int x1, int y1, int x2, int y2) {
    if (x1 < 0 || x1 >= SIZE || y1 < 0 || y1 >= SIZE || x2 < 0 || x2 >= SIZE || y2 < 0 || y2 >= SIZE) {
        return;
    }
    if (board[x1][y1] == nullptr) {
        return;
    }
    Piece* piece = board[x1][y1];
    if (board[x2][y2] != nullptr) {
        chess->deletePiece(board[x2][y2]);
    }
    else {
        if (piece->isPawn() && x2 == x1 + (piece->isWhite() ? 1 : -1) && y2 == y1 + 1) {
            if (board[x1][y1 + 1] != nullptr && board[x1][y1 + 1]->isWhite() != piece->isWhite()) {
                chess->deletePiece(board[x1][y1 + 1]);
                board[x1][y1 + 1] = nullptr;
            }
        }
        if (piece->isPawn() && x2 == x1 + (piece->isWhite() ? 1 : -1) && y2 == y1 - 1) {
            if (board[x1][y1 - 1] != nullptr && board[x1][y1 - 1]->isWhite() != piece->isWhite()) {
                chess->deletePiece(board[x1][y1 - 1]);
                board[x1][y1 - 1] = nullptr;
            }
        }
    }
    board[x2][y2] = piece;
    board[x1][y1] = nullptr;
    board[x2][y2]->setPosition(x2, y2);
    if (!board[x2][y2]->hasMoved()) {
        board[x2][y2]->move();
    }
    if (piece->isSwitchable()) {
        piece = chess->switchPiece(piece, 'Q');
    }
}

void Board::display() {
    // Affichage de l'échiquier
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << "[";
            if (board[i][j] == nullptr) {
                cout << " ";
            } else {
                cout << (board[i][j]->isWhite() ? "\033[34m" : "\033[31m");
                cout << board[i][j]->getSymbol();
                cout << "\033[0m";
            }
            cout << "]";
        }
        cout << endl;
    }
    cout << "\n" << endl;
}

Board* Board::clone(Chess* chess) {
    // Création d'une copie de l'échiquier
    Board* clone = blankBoard(chess);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] != nullptr) {
                clone->setPiece(i,j, board[i][j]->clone());
            }
        }
    }
    return clone;
}

void Board::setPiece(int x, int y, Piece* piece) {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || piece == nullptr) {
        return;
    }
    if (board[x][y] != nullptr) {
        chess->deletePiece(board[x][y]);
        delete board[x][y];
    }
    piece->setPosition(x, y);
    chess->addPiece(piece);
    board[x][y] = piece;
}

void Board::deleteBoard() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] != nullptr) {
                delete board[i][j];
            }
        }
    }
}

void Board::setBoard(vector<vector<Piece*>> board) {
    deleteBoard();
    this->board = board;
}

vector<vector<Piece*>> Board::getBoard() {
    return board;
}
