#ifndef HEURISTICS_HPP
#define HEURISTICS_HPP

#include "../../Chess.hpp"
#include "../../piece/Piece.hpp"
#include "../../board/Board.hpp"
#include <unordered_set> // std::unordered_set

class Chess;
class Piece;
class Board;

/**
 * @brief La classe Heuristics est une classe abstraite représentant une heuristique.
 */
class Heuristic {
    public:

        /**
         * @brief Destructeur de la classe Heuristic.
         */
        virtual ~Heuristic() {}

        /**
         * @brief Calcule la valeur de l'heuristique pour une partie d'échecs.
         * 
         * @param chess La situation actuelle.
         * @param isWhite true si le joueur est blanc, false sinon.
         * @param originalChess La situation originale.
         * 
         * @return La valeur de l'heuristique.
         */
        virtual int evaluate(Chess* chess, bool isWhite, Chess* originalChess) = 0;
};

/**
 * @brief La classe BasicHeuristic est une classe représentant une heuristique basique.
 */
class BasicHeuristic : public Heuristic {
    public:
        /**
         * @brief Constructeur de la classe BasicHeuristic.
         */
        BasicHeuristic();

        /**
         * @brief Destructeur de la classe BasicHeuristic.
         */
        ~BasicHeuristic();

        /**
         * @brief Calcule la valeur de l'heuristique pour une partie d'échecs.
         * 
         * @param chess La situation actuelle.
         * @param isWhite true si le joueur est blanc, false sinon.
         * @param originalChess La situation originale.
         * 
         * @return La valeur de l'heuristique.
         */
        int evaluate(Chess* chess, bool isWhite, Chess* originalChess) override;

        /**
         * @brief Récupère la valeur d'une pièce.
         * 
         * @param piece La pièce.
         * 
         * @return La valeur de la pièce.
         */
        int getValue(Piece* piece, Chess* chess);
};


#endif // HEURISTICS_HPP