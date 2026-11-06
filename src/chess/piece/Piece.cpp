#include "../../../include/chess/piece/Piece.hpp"
#include "../../../include/chess/board/Board.hpp"
#include <unordered_set> // std::unordered_set
#include <utility> // std::pair
#include <iostream> // std::cout, std::endl

using namespace std;

// Methodes propre à toute les pièces
AllPieces::AllPieces(bool isWhite) : white(isWhite) {}
AllPieces::AllPieces(bool isWhite, int x, int y) : white(isWhite) {
    position = {x,y};
}
bool AllPieces::isWhite() { 
    return white; 
}
pair<int,int> AllPieces::getPosition() { 
    return position; 
}
void AllPieces::setPosition(int x, int y) { 
    position = {x,y}; 
}
bool AllPieces::isSwitchable() { 
    return false; 
}
bool AllPieces::hasMoved() { 
    return moved; 
}
void AllPieces::move() { 
    moved = true; 
}
bool AllPieces::isKing() { 
    return (this->getSymbol() == 'K' || this->getSymbol() == 'k');
}

bool AllPieces::isRook() { 
    return (this->getSymbol() == 'R' || this->getSymbol() == 'r');
}

bool AllPieces::isBishop() { 
    return (this->getSymbol() == 'B' || this->getSymbol() == 'b');
}

bool AllPieces::isQueen() { 
    return (this->getSymbol() == 'Q' || this->getSymbol() == 'q');
}

bool AllPieces::isKnight() { 
    return (this->getSymbol() == 'C' || this->getSymbol() == 'c');
}

bool AllPieces::isPawn() { 
    return (this->getSymbol() == 'P' || this->getSymbol() == 'p');
}

bool AllPieces::isPassedPawn(Board* board) {
    if (!this->isPawn()) {
        return false; // Seuls les pions peuvent être des pions passés
    }

    int direction = this->isWhite() ? 1 : -1; // Déterminer la direction en fonction de la couleur

    int startX = this->getPosition().first;
    int startY = this->getPosition().second;

    for (int y = startY + direction; (direction == 1 ? y <= 7 : y >= 0); y += direction) {
        for (int x = startX - 1; x <= startX + 1; ++x) {
            if (x >= 0 && x <= 7) {
                Piece* piece = board->getPiece(x, y);
                if (piece != nullptr && piece->isPawn() && piece->isWhite() != this->isWhite()) {
                    return false; // Il y a un pion adverse dans la voie
                }
            }
        }
    }

    return true; // Aucun pion adverse dans la voie, c'est un pion passé
}

bool AllPieces::controlsCenter() {
    int x = position.first;
    int y = position.second;
    return (x >= 2 || x <= 5 ) && (y >= 2 || y <= 5);
}

// Methodes propre à chaque pièce //

// Pion

Pawn::Pawn(bool isWhite) : AllPieces(isWhite) {}
Pawn::~Pawn() {}
Pawn::Pawn(bool isWhite, int x, int y) : AllPieces(isWhite) {
    setPosition(x, y);
}
unordered_set<pair<int,int>, pair_hash> Pawn::getPossibleMoves(Board* board) {
    unordered_set<pair<int,int>, pair_hash> moves;
    int x = position.first;
    int y = position.second;
    int direction = this->isWhite() ? 1 : -1;
    if (x+direction >= 0 && x+direction < 8) {
        if (board->getPiece(x+direction, y) == nullptr) {
            moves.insert({x+direction, y});
            if ((x == 1 && this->isWhite()) || (x == 6 && !this->isWhite())) {
                if (board->getPiece(x+2*direction, y) == nullptr) {
                    moves.insert({x+2*direction, y});
                }
            }
        }
        if (y > 0 && 
            ((board->getPiece(x+direction, y-1) != nullptr && board->getPiece(x+direction, y-1)->isWhite() != this->isWhite()) || 
            (board->getPiece(x, y-1) != nullptr && board->getPiece(x, y-1)->isWhite() != this->isWhite() && board->getPiece(x+direction, y-1) == nullptr))) {
            moves.insert({x+direction, y-1});
        }
        if (y < 7 && 
            ((board->getPiece(x+direction, y+1) != nullptr && board->getPiece(x+direction, y+1)->isWhite() != this->isWhite()) || 
            (board->getPiece(x, y+1) != nullptr && board->getPiece(x, y+1)->isWhite() != this->isWhite() && board->getPiece(x+direction, y+1) == nullptr))){
            moves.insert({x+direction, y+1});
        }
    }
    return moves;
}
char Pawn::getSymbol() { return isWhite() ? 'P' : 'p'; }
Pawn* Pawn::clone() {
    Pawn* clone = new Pawn(this->isWhite());
    clone->setPosition(position.first, position.second);
    return clone;
}
bool Pawn::isSwitchable() { 
    if (this->isWhite() && position.first == 7) {
        return true;
    }
    if (!this->isWhite() && position.first == 0) {
        return true;
    }
    return false;
}

int Pawn::getPositionalValue(Board* board) {
    int x = position.first;
    int value = 0;
    for (int i = -1; i <= 1; i++) {
        if (position.second+i >= 0 && position.second+i < 8) {
            Piece* piece = board->getPiece(position.first, position.second+i);
            if (piece != nullptr && piece->isPawn() && piece->isWhite() == this->isWhite()) {
                value += 10;
            }
        }
    }
    return (isWhite() ? value + x : value + (7-x));
}

// Tour

Rook::Rook(bool isWhite) : AllPieces(isWhite) {}
Rook::~Rook() {}
Rook::Rook(bool isWhite, int x, int y) : AllPieces(isWhite) {
    setPosition(x, y);
}
unordered_set<pair<int,int>, pair_hash> Rook::getPossibleMoves(Board* board) {
    unordered_set<pair<int,int>, pair_hash> moves;
    int x = position.first;
    int y = position.second;
    for (int i = x+1; i < 8; i++) {
        if (board->getPiece(i, y) == nullptr) {
            moves.insert({i, y});
        } else {
            if (board->getPiece(i, y)->isWhite() != this->isWhite()) {
                moves.insert({i, y});
            }
            break;
        }
    }
    for (int i = x-1; i >= 0; i--) {
        if (board->getPiece(i, y) == nullptr) {
            moves.insert({i, y});
        } else {
            if (board->getPiece(i, y)->isWhite() != this->isWhite()) {
                moves.insert({i, y});
            }
            break;
        }
    }
    for (int j = y+1; j < 8; j++) {
        if (board->getPiece(x, j) == nullptr) {
            moves.insert({x, j});
        } else {
            if (board->getPiece(x, j)->isWhite() != this->isWhite()) {
                moves.insert({x, j});
            }
            break;
        }
    }
    for (int j = y-1; j >= 0; j--) {
        if (board->getPiece(x, j) == nullptr) {
            moves.insert({x, j});
        } else {
            if (board->getPiece(x, j)->isWhite() != this->isWhite()) {
                moves.insert({x, j});
            }
            break;
        }
    }
    return moves;
}
char Rook::getSymbol() { return isWhite() ? 'R' : 'r'; }
Rook* Rook::clone() {
    Rook* clone = new Rook(this->isWhite());
    clone->setPosition(position.first, position.second);
    return clone;
}
bool Rook::canCastle() { 
    return !moved; 
}

int Rook::getPositionalValue(Board* board) {
    int x = position.first;
    int value = 0;
    if (white) {
        if (x == 6 || x == 7) {
            value += 10;
        }
    } else {
        if (x == 0 || x == 1) {
            value += 10;
        }
    }
    return value;
}

// Cavalier

Knight::Knight(bool isWhite) : AllPieces(isWhite) {}
Knight::~Knight() {}
Knight::Knight(bool isWhite, int x, int y) : AllPieces(isWhite) {
    setPosition(x, y);
}
unordered_set<pair<int,int>, pair_hash> Knight::getPossibleMoves(Board* board) {
    unordered_set<pair<int,int>, pair_hash> moves;    
    int x = position.first;
    int y = position.second;
    int dx[] = {2, 1, -1, -2, -2, -1, 1, 2};
    int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};
    for (int i = 0; i < 8; i++) {
        if (x+dx[i] >= 0 && x+dx[i] < 8 && y+dy[i] >= 0 && y+dy[i] < 8 && (board->getPiece(x+dx[i], y+dy[i]) == nullptr || board->getPiece(x+dx[i], y+dy[i])->isWhite() != this->isWhite())) {
            moves.insert({x+dx[i], y+dy[i]});
        }
    }
    return moves;
}
char Knight::getSymbol() { return isWhite() ? 'C' : 'c'; }
Knight* Knight::clone() {
    Knight* clone = new Knight(this->isWhite());
    clone->setPosition(position.first, position.second);
    return clone;
}

int Knight::getPositionalValue(Board* board) {
    int x = position.first;
    int y = position.second;
    int value = 0;
    if (x==3 || x == 4) {
        value += 10;
    }
    if (y==3 || y == 4) {
        value += 10;
    }
    return value;
}

// Fou

Bishop::Bishop(bool isWhite) : AllPieces(isWhite) {}
Bishop::~Bishop() {}
Bishop::Bishop(bool isWhite, int x, int y) : AllPieces(isWhite) {
    setPosition(x, y);
}
unordered_set<pair<int,int>, pair_hash> Bishop::getPossibleMoves(Board* board) {
    unordered_set<pair<int,int>, pair_hash> moves;
    int x = position.first;
    int y = position.second;
    for (int i = 1; x+i < 8 && y+i < 8; i++) {
        if (board->getPiece(x+i, y+i) == nullptr) {
            moves.insert({x+i, y+i});
        } else {
            if (board->getPiece(x+i, y+i)->isWhite() != this->isWhite()) {
                moves.insert({x+i, y+i});
            }
            break;
        }
    }
    for (int i = 1; x+i < 8 && y-i >= 0; i++) {
        if (board->getPiece(x+i, y-i) == nullptr) {
            moves.insert({x+i, y-i});
        } else {
            if (board->getPiece(x+i, y-i)->isWhite() != this->isWhite()) {
                moves.insert({x+i, y-i});
            }
            break;
        }
    }
    for (int i = 1; x-i >= 0 && y+i < 8; i++) {
        if (board->getPiece(x-i, y+i) == nullptr) {
            moves.insert({x-i, y+i});
        } else {
            if (board->getPiece(x-i, y+i)->isWhite() != this->isWhite()) {
                moves.insert({x-i, y+i});
            }
            break;
        }
    }
    for (int i = 1; x-i >= 0 && y-i >= 0; i++) {
        if (board->getPiece(x-i, y-i) == nullptr) {
            moves.insert({x-i, y-i});
        } else {
            if (board->getPiece(x-i, y-i)->isWhite() != this->isWhite()) {
                moves.insert({x-i, y-i});
            }
            break;
        }
    }
    return moves;
}
char Bishop::getSymbol() { return isWhite() ? 'B' : 'b'; }
Bishop* Bishop::clone() {
    Bishop* clone = new Bishop(this->isWhite());
    clone->setPosition(position.first, position.second);
    return clone;
}

int Bishop::getPositionalValue(Board* board) {
    int x = position.first;
    int y = position.second;
    int value = 0;
    if (x==3 || x == 4) {
        value += 10;
    }
    if (y==3 || y == 4) {
        value += 10;
    }
    for (int i = -1; i <= 1; i++) {
        if (position.second+i >= 0 && position.second+i < 8) {
            Piece* piece = board->getPiece(position.first, position.second+i);
            if (piece != nullptr && piece->isBishop() && piece->isWhite() == this->isWhite()) {
                value += 10;
            }
        }
    }
    return value;
}

// Reine

Queen::Queen(bool isWhite) : AllPieces(isWhite) {}
Queen::~Queen() {}
Queen::Queen(bool isWhite, int x, int y) : AllPieces(isWhite) {
    setPosition(x, y);
}
unordered_set<pair<int,int>, pair_hash> Queen::getPossibleMoves(Board* board) {
    unordered_set<pair<int,int>, pair_hash> moves;
    int x = position.first;
    int y = position.second;
    for (int i = x+1; i < 8; i++) {
        if (board->getPiece(i, y) == nullptr) {
            moves.insert({i, y});
        } else {
            if (board->getPiece(i, y)->isWhite() != this->isWhite()) {
                moves.insert({i, y});
            }
            break;
        }
    }
    for (int i = x-1; i >= 0; i--) {
        if (board->getPiece(i, y) == nullptr) {
            moves.insert({i, y});
        } else {
            if (board->getPiece(i, y)->isWhite() != this->isWhite()) {
                moves.insert({i, y});
            }
            break;
        }
    }
    for (int j = y+1; j < 8; j++) {
        if (board->getPiece(x, j) == nullptr) {
            moves.insert({x, j});
        } else {
            if (board->getPiece(x, j)->isWhite() != this->isWhite()) {
                moves.insert({x, j});
            }
            break;
        }
    }
    for (int j = y-1; j >= 0; j--) {
        if (board->getPiece(x, j) == nullptr) {
            moves.insert({x, j});
        } else {
            if (board->getPiece(x, j)->isWhite() != this->isWhite()) {
                moves.insert({x, j});
            }
            break;
        }
    }
    for (int i = 1; x+i < 8 && y+i < 8; i++) {
        if (board->getPiece(x+i, y+i) == nullptr) {
            moves.insert({x+i, y+i});
        } else {
            if (board->getPiece(x+i, y+i)->isWhite() != this->isWhite()) {
                moves.insert({x+i, y+i});
            }
            break;
        }
    }
    for (int i = 1; x+i < 8 && y-i >= 0; i++) {
        if (board->getPiece(x+i, y-i) == nullptr) {
            moves.insert({x+i, y-i});
        } else {
            if (board->getPiece(x+i, y-i)->isWhite() != this->isWhite()) {
                moves.insert({x+i, y-i});
            }
            break;
        }
    }
    for (int i = 1; x-i >= 0 && y+i < 8; i++) {
        if (board->getPiece(x-i, y+i) == nullptr) {
            moves.insert({x-i, y+i});
        } else {
            if (board->getPiece(x-i, y+i)->isWhite() != this->isWhite()) {
                moves.insert({x-i, y+i});
            }
            break;
        }
    }
    for (int i = 1; x-i >= 0 && y-i >= 0; i++) {
        if (board->getPiece(x-i, y-i) == nullptr) {
            moves.insert({x-i, y-i});
        } else {
            if (board->getPiece(x-i, y-i)->isWhite() != this->isWhite()) {
                moves.insert({x-i, y-i});
            }
            break;
        }
    }
    return moves;
}
char Queen::getSymbol() { return isWhite() ? 'Q' : 'q'; }
Queen* Queen::clone() {
    Queen* clone = new Queen(this->isWhite());
    clone->setPosition(position.first, position.second);
    return clone;
}

int Queen::getPositionalValue(Board* board) {
    int x = position.first;
    int y = position.second;
    int value = 0;
    if (x==3 || x == 4) {
        value += 10;
    }
    if (y==3 || y == 4) {
        value += 10;
    }
    return value;
}

// Roi

King::King(bool isWhite) : AllPieces(isWhite) {}
King::~King() {}
King::King(bool isWhite, int x, int y) : AllPieces(isWhite) {
    setPosition(x, y);
}
unordered_set<pair<int,int>, pair_hash> King::getPossibleMoves(Board* board) {
    unordered_set<pair<int,int>, pair_hash> moves;
    int x = position.first;
    int y = position.second;
    int dx[] = {1, 1, 1, 0, 0, -1, -1, -1};
    int dy[] = {1, 0, -1, 1, -1, 1, 0, -1};
    for (int i = 0; i < 8; i++) {
        if (x+dx[i] >= 0 && x+dx[i] < 8 && y+dy[i] >= 0 && y+dy[i] < 8 && (board->getPiece(x+dx[i], y+dy[i]) == nullptr || board->getPiece(x+dx[i], y+dy[i])->isWhite() != this->isWhite())) {
            bool safe = true;
            for (int xi = (x+dx[i]-1 >=0 ? -1 : 0); xi <= (x+dx[i]+1 < 8 ? 1 : 0); xi++) {
                for (int yj = (y+dy[i]-1 >=0 ? -1 : 0); yj <= (y+dy[i]+1 < 8 ? 1 : 0); yj++) {
                    if (board->getPiece(x+dx[i]+xi, y+dy[i]+yj) != nullptr && board->getPiece(x+dx[i]+xi, y+dy[i]+yj)->isKing() && board->getPiece(x+dx[i]+xi, y+dy[i]+yj)->isWhite() != this->isWhite()){
                        safe = false;
                        break;
                    }
                }
                if (!safe) {
                    break;
                }
            }
            if (safe) {
                moves.insert({x+dx[i], y+dy[i]});
            }
        }
    }
    return moves;
}
char King::getSymbol() { return isWhite() ? 'K' : 'k'; }
King* King::clone() {
    King* clone = new King(this->isWhite());
    clone->setPosition(position.first, position.second);
    return clone;
}
bool King::canCastle() { 
    return !moved; 
}

int King::getPositionalValue(Board* board) {
    int value = 0;
    for (int i = -1; i <= 1; i++) {
        if (position.second+i >= 0 && position.second+i < 8) {
            Piece* piece = board->getPiece(position.first, position.second+i);
            if (piece != nullptr && piece->isPawn() && piece->isWhite() == this->isWhite()) {
                value += 10;
            }
        }
    }
    return value;
}
