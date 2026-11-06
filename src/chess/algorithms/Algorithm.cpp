#include "../../../include/chess/algorithms/Algorithm.hpp"
#include "../../../include/chess/algorithms/heuristics/Heuristics.hpp"
#include "../../../include/chess/piece/Piece.hpp"
#include "../../../include/chess/Chess.hpp"
#include <vector> // std::vector 
#include <unordered_set> // std::unordered_set
#include <unordered_map> // std::unordered_map
#include <iostream> // std::cout, std::endl
#include <omp.h> // OpenMP
#include <chrono>
#include <algorithm> // std::sort

using namespace std;

vector<Piece*> toVector(unordered_set<Piece*> pieces) {
    return vector<Piece*>(pieces.begin(), pieces.end());
}

vector<pair<int,int>> toVectorBis(unordered_set<pair<int,int>, pair_hash> moves) {
    return vector<pair<int,int>>(moves.begin(), moves.end());
}


vector<char> Algorithm::getSymbols() {
    return vector<char> {'R', 'N', 'B', 'Q'};
}

Heuristic* Algorithm::getHeuristic() {
    return heuristic;
}

void Algorithm::setHeuristic(Heuristic* heuristic) {
    this->heuristic = heuristic;
}

size_t Algorithm::hashChess(Chess* chess) {
    size_t hash = 0;
    for (Piece* piece : chess->getWhitePieces()) {
        hash += piece->getSymbol();
        hash += piece->getPosition().first;
        hash += piece->getPosition().second;
    }
    for (Piece* piece : chess->getBlackPieces()) {
        hash += piece->getSymbol();
        hash += piece->getPosition().first;
        hash += piece->getPosition().second;
    }
    hash += chess->getWhitePieces().size();
    hash += chess->getBlackPieces().size();
    return hash;
}

//////////////////////////////////////////////
///////////////// MiniMax ////////////////////
//////////////////////////////////////////////

MiniMax::MiniMax(int depth, Heuristic* heuristic) {
    this->depth = depth;
    this->heuristic = heuristic;
}

MiniMax::~MiniMax() {
    delete heuristic;
}


pair<Piece*, pair<int,int>> MiniMax::getBestMove(Chess* chess, bool isWhite) {
    int bestScore = -INFINITY;
    pair<int,int> bestMove;
    Piece* bestPieceToMove = nullptr;
    Chess* newChess = nullptr;
    // double percentage = 0.0;

    auto start = std::chrono::high_resolution_clock::now();

    vector<Piece*> pieces = toVector(isWhite ? chess->getWhitePieces() : chess->getBlackPieces());
    sort(pieces.begin(), pieces.end(), [chess](Piece* piece1, Piece* piece2) { return piece1->getPositionalValue(chess->getBoard()) < piece2->getPositionalValue(chess->getBoard()); });
    
#pragma omp parallel for default(none) shared(bestMove, bestPieceToMove, newChess, pieces, chess, isWhite) \
    schedule(dynamic) reduction(max:bestScore)
    for (size_t i = 0; i < pieces.size(); i++) {
        Piece* piece = pieces[i];
        vector<pair<int,int>> moves = toVectorBis(piece->getPossibleMoves(chess->getBoard()));
        for (size_t j = 0; j < moves.size(); j++) {
            pair<int,int> move = moves[j];
            // std::cout << "Percentage done : " << percentage << "%" << std::endl;
            newChess = chess->clone();    
            newChess->movePiece(piece->getPosition().first, piece->getPosition().second, move.first, move.second);

            if (newChess->isCheck(isWhite)) {
                continue;
            }

            if (piece->isSwitchable()) {
                newChess->switchPiece(piece,'Q');
                int score = minimax(newChess, false, isWhite, depth, chess);

                if (score > bestScore) {
                    #pragma omp critical
                    {
                        bestScore = score;
                        bestMove = move;
                        bestPieceToMove = piece;
                    }
                }
            }
            else {
                int score = minimax(newChess, false, isWhite, depth, chess);

                if (score > bestScore) {
                    #pragma omp critical
                    {
                        bestScore = score;
                        bestMove = move;
                        bestPieceToMove = piece;
                    }
                }
            }

            // percentage += 100/(double)(moves.size() * (isWhite ? chess->getWhitePieces().size() : chess->getBlackPieces().size()));
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Temps d'exécution : " << duration.count() << " ms" << std::endl;

    delete newChess;
    return pair<Piece*, pair<int,int>>(bestPieceToMove, bestMove);
}

int MiniMax::minimax(Chess* chess, bool myTurn, bool myColor, int depth, Chess* originalChess) {
    if (chess->isStalemate((myTurn ? myColor : !myColor))) {
        return 0;
    }
    if (chess->isCheck((myTurn ? myColor : !myColor))) {
        return myTurn ? -INFINITY+1 : INFINITY-1;
    }
    if (depth == 0) {
        return heuristic->evaluate(chess, (myTurn ? myColor : !myColor), originalChess);
    }

    // std::cout << ((myTurn ? myColor : !myColor) ? "White" : "Black") << " turn\n";

    int bestScore = myTurn ? -INFINITY : INFINITY;

    for (bool isKingSide : {true, false}) {
        if (chess->canCastle((myTurn ? myColor : !myColor), isKingSide)) {
            Chess* newChess = new Chess(chess);
            newChess->makeCastle((myTurn ? myColor : !myColor), isKingSide);
            int score = minimax(newChess, !myTurn, myColor, depth - 1, originalChess);
            if (myTurn) {
                bestScore = max(bestScore, score);
            } else {
                bestScore = min(bestScore, score);
            }
            delete newChess;
        }
    }

    vector<Piece*> pieces = toVector((myTurn ? myColor : !myColor) ? chess->getWhitePieces() : chess->getBlackPieces());
    sort(pieces.begin(), pieces.end(), [chess](Piece* piece1, Piece* piece2) { return piece1->getPositionalValue(chess->getBoard()) < piece2->getPositionalValue(chess->getBoard()); });


    for (Piece* piece : pieces) {
        // std::cout << "Piece : " << piece->getSymbol() << std::endl;
        unordered_set<pair<int,int>, pair_hash> moves = piece->getPossibleMoves(chess->getBoard());
        for (pair<int,int> move : moves) {
            Chess* clonedChess = new Chess(chess);
            clonedChess->getBoard()->movePiece(piece->getPosition().first, piece->getPosition().second, move.first, move.second);

            if (clonedChess->isCheck((myTurn ? myColor : !myColor))) {
                continue;
            }

            int score = minimax(clonedChess, !myTurn, myColor, depth - 1, originalChess);

            if (myTurn) {
                bestScore = max(bestScore, score);
            } else {
                bestScore = min(bestScore, score);
            }
            
            delete clonedChess;
        }
    }
    return bestScore;
}

//////////////////////////////////////////////
//////////////// AlphaBeta ///////////////////
//////////////////////////////////////////////

AlphaBeta::AlphaBeta(int depth, Heuristic* heuristic) {
    this->depth = depth;
    this->heuristic = heuristic;
    transpositionTable = unordered_map<size_t, TranspositionEntry>();
}

AlphaBeta::~AlphaBeta() {
    delete heuristic;
}

std::pair<Piece*, std::pair<int, int>> AlphaBeta::getBestMove(Chess* chess, bool isWhite) {
    int bestScore = -INFINITY;
    std::pair<int, int> bestMove;
    Piece* bestPieceToMove = nullptr;

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<Piece*> pieces = toVector(isWhite ? chess->getWhitePieces() : chess->getBlackPieces());

#pragma omp parallel
    {
        Chess* newChess = nullptr;
        int localBestScore = -INFINITY;
        std::pair<int, int> localBestMove;
        Piece* localBestPieceToMove = nullptr;

#pragma omp for schedule(dynamic)
        for (size_t i = 0; i < pieces.size(); i++) {
            Piece* piece = pieces[i];
            std::vector<std::pair<int, int>> moves = toVectorBis(piece->getPossibleMoves(chess->getBoard()));
            for (size_t j = 0; j < moves.size(); j++) {
                std::pair<int, int> move = moves[j];
                newChess = chess->clone();
                newChess->movePiece(piece->getPosition().first, piece->getPosition().second, move.first, move.second);

                if (newChess->isCheck(isWhite)) {
                    delete newChess;
                    continue;
                }

                int score;
                if (piece->isSwitchable()) {
                    newChess->switchPiece(piece, 'Q');
                    score = alphabeta(newChess, false, isWhite, depth, chess, -INFINITY, INFINITY);
                } else {
                    score = alphabeta(newChess, false, isWhite, depth, chess, -INFINITY, INFINITY);
                }

                if (score > localBestScore) {
                    localBestScore = score;
                    localBestMove = move;
                    localBestPieceToMove = piece;
                }

                delete newChess;
            }
        }

#pragma omp critical
        {
            if (localBestScore > bestScore) {
                bestScore = localBestScore;
                bestMove = localBestMove;
                bestPieceToMove = localBestPieceToMove;
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Temps d'exécution : " << duration.count() << " ms" << std::endl;

    return std::make_pair(bestPieceToMove, bestMove);
}

int AlphaBeta::alphabeta(Chess* chess, bool myTurn, bool myColor, int depth, Chess* originalChess, int alpha, int beta) {
    size_t hash = hashChess(chess);

    if (transpositionTable.find(hash) != transpositionTable.end()) {
        TranspositionEntry entry = transpositionTable[hash];
        if (entry.depth >= depth) {
            if (entry.alpha <= alpha && entry.beta >= beta) {
                return entry.score;
            }
        }
    }

    if (chess->isStalemate((myTurn ? myColor : !myColor))) {
        return 0;
    }
    if (chess->isCheck((myTurn ? myColor : !myColor))) {
        return myTurn ? -INFINITY+1 : INFINITY-1;
    }
    if (depth == 0) {
        return heuristic->evaluate(chess, (myTurn ? myColor : !myColor), originalChess);
    }

    // std::cout << ((myTurn ? myColor : !myColor) ? "White" : "Black") << " turn\n";

    int bestScore = myTurn ? -INFINITY : INFINITY;

    for (bool isKingSide : {true, false}) {
        if (chess->canCastle((myTurn ? myColor : !myColor), isKingSide)) {
            Chess* newChess = new Chess(chess);
            newChess->makeCastle((myTurn ? myColor : !myColor), isKingSide);
            int score = alphabeta(newChess, !myTurn, myColor, depth - 1, originalChess, alpha, beta);
            if (myTurn) {
                bestScore = max(bestScore, score);
                alpha = max(alpha, bestScore);
            } else {
                bestScore = min(bestScore, score);
                beta = min(beta, bestScore);
            }
            delete newChess;
            if (beta <= alpha) {
                break;
            }
        }
    }

    for (Piece* piece : (myTurn ? myColor : !myColor) ? chess->getWhitePieces() : chess->getBlackPieces()) {
        // std::cout << "Piece : " << piece->getSymbol() << std::endl;
        unordered_set<pair<int,int>, pair_hash> moves = piece->getPossibleMoves(chess->getBoard());
        for (pair<int,int> move : moves) {
            Chess* clonedChess = new Chess(chess);
            clonedChess->getBoard()->movePiece(piece->getPosition().first, piece->getPosition().second, move.first, move.second);

            if (clonedChess->isCheck((myTurn ? myColor : !myColor))) {
                continue;
            }

            int score = alphabeta(clonedChess, !myTurn, myColor, depth - 1, originalChess, alpha, beta);

            if (myTurn) {
                bestScore = max(bestScore, score);
                alpha = max(alpha, bestScore);
            } else {
                bestScore = min(bestScore, score);
                beta = min(beta, bestScore);
            }

            delete clonedChess;

            if (beta <= alpha) {
                break;
            }
        }
    }
    TranspositionEntry entry = { depth, bestScore, alpha, beta };
    transpositionTable[hash] = entry;

    return bestScore;
}



