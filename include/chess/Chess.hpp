#ifndef CHESS_HPP
#define CHESS_HPP

#include "board/Board.hpp"
#include "piece/Piece.hpp"
#include "algorithms/Algorithm.hpp"
#include <vector> // std::vector
#include <unordered_set> // std::unordered_set

class Board;
class Piece;
class Algorithm;

/**
 * @brief La classe Chess représente une partie d'échecs.
 * 
 * Cette classe contient un échiquier et les pièces des deux joueurs.
 */
class Chess {
    private:
        std::unordered_set<Piece*> whitePieces;
        std::unordered_set<Piece*> blackPieces;
        Board* board = nullptr;
    public:
        /**
         * @brief Constructeur de la classe Chess.
         */
        Chess(bool blank = false);

        /**
         * @brief Constructeur de la classe Chess.
         * 
         * @param chess Un pointeur vers une partie d'échecs.
         */
        Chess(Chess* chess);

        /**
         * @brief Destructeur de la classe Chess.
         */
        ~Chess();

        /**
         * @brief Crée une partie d'échecs vide.
         * 
         * @return Un pointeur vers une partie d'échecs vide.
         */
        static Chess* blankChess();

        /**
         * @brief Clone une partie d'échecs.
         * 
         * @return Un pointeur vers une copie de la partie d'échecs.
         */
        Chess* clone();

        /**
         * @brief Vérifie si un joueur est en échec.
         * 
         * @param isWhite true si le joueur est blanc, false sinon.
         * @return true si le joueur est en échec, false sinon.
         */
        bool isCheck(bool isWhite);

        /**
         * @brief Vérifie si un joueur est en échec et mat.
         * 
         * @param isWhite true si le joueur est blanc, false sinon.
         * @return true si le joueur est en échec et mat, false sinon.
         */
        bool isCheckmate(bool isWhite);

        /**
         * @brief Vérifie si la partie est nulle.
         * 
         * @param isWhite true si le joueur est blanc, false sinon.
         * @return true si la partie est nulle, false sinon.
         */
        bool isStalemate(bool isWhite);
        
        /**
         * @brief Récupère l'échiquier.
         * 
         * @return Un pointeur vers l'échiquier.
         */
        Board* getBoard();

        /**
         * @brief Récupère les pièces blanches.
         * 
         * @return Un vecteur de pointeurs vers les pièces blanches.
         */
        std::unordered_set<Piece*> getWhitePieces();

        /**
         * @brief Récupère les pièces noires.
         * 
         * @return Un vecteur de pointeurs vers les pièces noires.
         */
        std::unordered_set<Piece*> getBlackPieces();

        /**
         * @brief Ajoute une pièce blanche.
         * 
         * @param piece Un pointeur vers la pièce à ajouter.
         */
        void addWhitePiece(Piece* piece);

        /**
         * @brief Ajoute une pièce noire.
         * 
         * @param piece Un pointeur vers la pièce à ajouter.
         */ 
        void addBlackPiece(Piece* piece);

        /**
         * @brief Joue une partie d'échecs.
         * 
         * @param algorithm1 Un pointeur vers l'algorithme du joueur Blanc.
         * @param algorithm2 Un pointeur vers l'algorithme du joueur Noir.
         */
        void play(Algorithm* algorithm1, Algorithm* algorithm2);

        /**
         * @brief Définit l'échiquier.
         * 
         * @param board Un pointeur vers l'échiquier.
         */
        void setBoard(Board* board);

        /**
         * @brief Change une pièce.
         * 
         * @param piece Un pointeur vers la pièce à changer.
         * @param symbol Le symbole de la nouvelle pièce.
         */
        Piece* switchPiece(Piece* piece, char symbol);

        /**
         * @brief Déplace une pièce.
         * 
         * @param piece Un pointeur vers la pièce à déplacer.
         * @param x La colonne de la position d'arrivée de la pièce.
         * @param y La ligne de la position d'arrivée de la pièce.
         */
        void movePiece(int i, int j, int x, int y);

        /**
         * @brief Supprime une pièce.
         * 
         * @param piece Un pointeur vers la pièce à supprimer.
         */
        void deletePiece(Piece* piece);

        /**
         * @brief Ajoute une pièce.
         * 
         * @param piece Un pointeur vers la pièce à ajouter.
         */
        void addPiece(Piece* piece);

        /**
         * @brief Vérifie si un roque est possible.
         * 
         * @param isWhite Vrai si le joueur est blanc, faux sinon.
         * @param isKingSide Vrai si le roque est grand, faux sinon.
         * @return Vrai si le roque est possible, faux sinon.
         */
        bool canCastle(bool isWhite, bool isKingSide);

        /**
         * @brief Effectue un roque.
         * 
         * @param isWhite Vrai si le joueur est blanc, faux sinon.
         * @param isKingSide Vrai si le roque est grand, faux sinon.
         */
        void makeCastle(bool isWhite, bool isKingSide);

        /**
         * @brief Récupère les pièces blanches.
         * 
         * @param whitePieces Un vecteur de pointeurs vers les pièces blanches.
         */
        void setWhitePieces(std::unordered_set<Piece*> whitePieces);

        /**
         * @brief Récupère les pièces noires.
         * 
         * @param blackPieces Un vecteur de pointeurs vers les pièces noires.
         */
        void setBlackPieces(std::unordered_set<Piece*> blackPieces);

        /**
         * @brief Place une pièce à une position donnée.
         * 
         * @param x La colonne de la position de la pièce.
         * @param y La ligne de la position de la pièce.
         * @return Un pointeur vers la pièce à la position (x, y).
         */
        void setPiece(int x, int y, Piece* piece);
};

#endif // CHESS_HPP