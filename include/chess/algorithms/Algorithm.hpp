#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "../Chess.hpp"
#include "../piece/Piece.hpp"
#include "./heuristics/Heuristics.hpp"
#include <vector> // std::vector
#include <climits> // INT_MAX
#include <unordered_map> // std::unordered_map

class Chess;
class Piece;
class Heuristic;

class Algorithm {
    protected:
        Heuristic* heuristic;

    public:
        /**
         * @brief Destructeur de la classe Algorithm.
         */
        virtual ~Algorithm() {}

        /**
         * @brief Récupère l'heuristique.
         * 
         * @return L'heuristique.
         */
        Heuristic* getHeuristic();

        /**
         * @brief Définit l'heuristique.
         * 
         * @param heuristic L'heuristique.
         */
        void setHeuristic(Heuristic* heuristic);

        /**
         * @brief Récupère le meilleur coup à jouer.
         * 
         * @param chess La partie d'échecs.
         * @param isWhite true si le joueur est blanc, false sinon.
         * 
         * @return Un vecteur contenant les coordonnées du meilleur coup.
         */
        virtual std::pair<Piece*,std::pair<int,int>> getBestMove(Chess* chess, bool isWhite) = 0;

        /**
         * @brief Récupère les symboles des pièces.
         * 
         * @return Un vecteur contenant les symboles des pièces.
         */
        std::vector<char> getSymbols();

        /**
         * @brief Hash une situation d'échecs.
         * 
         * @param chess La situation d'échecs.
        */
        size_t hashChess(Chess* chess);
};

struct TranspositionEntry {
    int depth;    // Profondeur à laquelle cette position a été évaluée
    int score;    // Score évalué
    int alpha;    // Valeur alpha lors de l'évaluation
    int beta;     // Valeur beta lors de l'évaluation
};

class MiniMax : public Algorithm {
    private:
        int depth;
        const int INFINITY = INT_MAX;
        
    public:
        /**
         * @brief Constructeur de la classe MiniMax.
         */
        MiniMax(int depth, Heuristic* heuristic);

        /**
         * @brief Destructeur de la classe MiniMax.
         */
        ~MiniMax();

        /**
         * @brief Récupère le meilleur coup à jouer.
         * 
         * @param chess La partie d'échecs.
         * @param isWhite true si le joueur est blanc, false sinon.
         * @return Un vecteur contenant les coordonnées du meilleur coup.
         */
        std::pair<Piece*,std::pair<int,int>> getBestMove(Chess* chess, bool isWhite) override;

        /**
         * @brief Déroule récursivement l'algorithme minimax.
         * 
         * @param chess La partie d'échecs.
         * @param myTurn true si c'est mon tour, false sinon.
         * @param myColor true si je suis blanc, false sinon.
         * @param depth La profondeur de l'algorithme.
         * @param originalChess La situation originale.
         * @return Le score du meilleur coup.
        */
        int minimax(Chess* chess, bool myTurn, bool myColor, int depth, Chess* originalChess);
};

class AlphaBeta : public Algorithm{
    private:
        int depth;
        const int INFINITY = INT_MAX;
        std::unordered_map<size_t, TranspositionEntry> transpositionTable;

    public:
        /**
         * @brief Constructeur de la classe AlphaBeta.
         */
        AlphaBeta(int depth, Heuristic* heuristic);

        /**
         * @brief Destructeur de la classe AlphaBeta.
         */
        ~AlphaBeta();

        /**
         * @brief Récupère le meilleur coup à jouer.
         * 
         * @param chess La partie d'échecs.
         * @param isWhite true si le joueur est blanc, false sinon.
         * @return Un vecteur contenant les coordonnées du meilleur coup.
         */
        std::pair<Piece*,std::pair<int,int>> getBestMove(Chess* chess, bool isWhite) override;

        /**
         * @brief Déroule récursivement l'algorithme alpha-beta.
         * 
         * @param chess La partie d'échecs.
         * @param myTurn true si c'est mon tour, false sinon.
         * @param myColor true si je suis blanc, false sinon.
         * @param depth La profondeur de l'algorithme.
         * @param originalChess La situation originale.
         * @param alpha La valeur alpha.
         * @param beta La valeur beta.
         * @return Le score du meilleur coup.
        */
        int alphabeta(Chess* chess, bool myTurn, bool myColor, int depth, Chess* originalChess, int alpha, int beta);
};

#endif