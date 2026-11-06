#include "../../../../include/chess/algorithms/heuristics/Heuristics.hpp"
#include "../../../../include/chess/piece/Piece.hpp"
#include "../../../../include/chess/Chess.hpp"

BasicHeuristic::BasicHeuristic() {}

BasicHeuristic::~BasicHeuristic() {}

int BasicHeuristic::evaluate(Chess* chess, bool isWhite, Chess* originalChess) {
    int score = 0;

    for (Piece* piece : (isWhite ? chess->getWhitePieces() : chess->getBlackPieces())) {
        score += getValue(piece, chess);
        score += piece->getPositionalValue(chess->getBoard());
    }
    for (Piece* piece : (isWhite ? chess->getBlackPieces() : chess->getWhitePieces())) {
        score -= getValue(piece, chess);
        score -= piece->getPositionalValue(chess->getBoard());
    }

    if ((isWhite ? chess->getBlackPieces().size() : chess->getWhitePieces().size()) < (isWhite ? originalChess->getBlackPieces().size() : originalChess->getBlackPieces().size()))
        score += 10 * (isWhite ? chess->getWhitePieces().size() : chess->getBlackPieces().size());
    if ((isWhite ? chess->getWhitePieces().size() : chess->getBlackPieces().size()) < (isWhite ? originalChess->getWhitePieces().size() : originalChess->getWhitePieces().size()))
        score -= 10 * (isWhite ? chess->getBlackPieces().size() : chess->getWhitePieces().size());

    if (chess->isCheck(!isWhite))
        score += 100;

    if (chess->isCheckmate(!isWhite))
        score += 1000000;

    // Penalty for repeating moves
    // if (chess->isThreefoldRepetition())
    //     score -= 1000;

    return score;
}

int BasicHeuristic::getValue(Piece* piece, Chess* chess) {
    if (piece->isPawn() && piece->isPassedPawn(chess->getBoard())) {
        return 20 + piece->getPossibleMoves(chess->getBoard()).size();
    } else if (piece->isBishop()) {
        return 30 + piece->getPossibleMoves(chess->getBoard()).size();
    } else if (piece->isKnight()) {
        return 30 + piece->getPossibleMoves(chess->getBoard()).size();
    } else if (piece->isRook()) {
        return 50 + piece->getPossibleMoves(chess->getBoard()).size();
    } else if (piece->isQueen()) {
        return 90 + piece->getPossibleMoves(chess->getBoard()).size();
    } else if (piece->isKing()) {
        return 1000 + piece->getPossibleMoves(chess->getBoard()).size();
    } else {
        return 10 + piece->getPossibleMoves(chess->getBoard()).size();
    }
}
