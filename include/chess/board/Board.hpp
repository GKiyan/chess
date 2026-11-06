#ifndef BOARD_HPP
#define BOARD_HPP

#include "../piece/Piece.hpp"
#include "../Chess.hpp"
#include <vector> // std::vector

class Piece;
class Chess;

/**
 * @brief La classe Board représente un échiquier.
 * 
 * Cette classe contient un tableau de pointeurs vers des pièces d'échecs.
 */
class Board {
    private:
        std::vector<std::vector<Piece*>> board; // Tableau de pointeurs vers des pièces
        Chess* chess;

        /**
         * @brief Supprime l'échiquier.
         */
        void deleteBoard();
        
    public:
        static const int SIZE = 8; // Taille de l'échiquier

        /**
         * @brief Constructeur de la classe Board.
         */
        Board(Chess* chess, bool blank = false);

        /**
         * @brief Destructeur de la classe Board.
         */
        ~Board();


        /**
         * @brief Crée un échiquier vide.
         * 
         * @return Un pointeur vers un échiquier vide.
         */
        static Board* blankBoard(Chess* chess); 

        /**
         * @brief Récupère la pièce à une position donnée.
         * 
         * @param x La colonne de la position de la pièce.
         * @param y La ligne de la position de la pièce.
         * @return Un pointeur vers la pièce à la position (x, y).
         */
        Piece* getPiece(int x, int y);

        /**
         * @brief Déplace une pièce d'une position à une autre.
         * 
         * @param x1 La colonne de la position de départ de la pièce.
         * @param y1 La ligne de la position de départ de la pièce.
         * @param x2 La colonne de la position d'arrivée de la pièce.
         * @param y2 La ligne de la position d'arrivée de la pièce.
         */
        void movePiece(int x1, int y1, int x2, int y2);

        /**
         * @brief Affiche l'échiquier.
         */
        void display();

        /**
         * @brief Clone l'échiquier.
         * 
         * @param chess Un pointeur vers la partie d'échecs.
         * @return Un pointeur vers une copie de l'échiquier.
         */
        Board* clone(Chess* chess);

        /**
         * @brief Récupère la partie d'échecs.
         * 
         * @return Un pointeur vers la partie d'échecs.
         */
        Chess* getChess();

        /**
         * @brief Définit une pièce à une position donnée.
         * 
         * @param x La colonne de la position de la pièce.
         * @param y La ligne de la position de la pièce.
         * @param piece Un pointeur vers la pièce à définir.
         */
        void setPiece(int x, int y, Piece* piece);
                
        /**
         * @brief Définit l'échiquier.
         * 
         * @param board Un vecteur de vecteurs de pointeurs vers des pièces.
         */
        void setBoard(std::vector<std::vector<Piece*>> board);  

        /**
         * @brief Récupère le tableau de pièces.
         * 
         * @return Un vecteur de vecteurs de pointeurs vers des pièces.
         */ 
        std::vector<std::vector<Piece*>> getBoard();
};

#endif // BOARD_HPP