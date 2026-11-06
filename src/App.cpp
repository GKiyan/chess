#include "../include/chess/board/Board.hpp"
#include "../include/chess/piece/Piece.hpp"
#include "../include/chess/Chess.hpp"
#include "../include/chess/algorithms/Algorithm.hpp"
#include "../include/chess/algorithms/heuristics/Heuristics.hpp"
#include <vector> // std::vector
#include <iostream>

using namespace std;


int main() {
    
    Chess* chess = new Chess();

    Board* board = chess->getBoard();

    board->display();

    Heuristic* heuristic = new BasicHeuristic();

    Algorithm* algorithm1 = new AlphaBeta(2, heuristic);
    Algorithm* algorithm2 = new AlphaBeta(2, heuristic);

    chess->play(algorithm1, algorithm2);

    delete chess;
    delete algorithm1;
    delete algorithm2;
    
    return 0;
}
