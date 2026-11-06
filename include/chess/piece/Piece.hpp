#ifndef PIECE_HPP
#define PIECE_HPP

#include <vector> // std::vector
#include <unordered_set> // std::unordered_set
#include "../board/Board.hpp"

class Board;

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second); 

        // Mainly for demonstration purposes, i.e. works but is overly simple
        // In the real world, use sth. like boost.hash_combine
        return h1 ^ h2;  
    }
};

/**
 * @brief La classe Piece est une classe abstraite représentant une pièce d'échecs.
 */
class Piece {
    public:      

        struct Hash {
            size_t operator()(const std::pair<int,int>& p) const {
                return std::hash<int>()(p.first) ^ std::hash<int>()(p.second);
            }
        };
          
        /**
         * @brief Destructeur virtuel de la classe Piece.
         */
        virtual ~Piece() {}

        /**
         * @brief Récupérer le symbole de la pièce.
         * @return Un caractère représentant le symbole de la pièce.
         */
        virtual char getSymbol() = 0;

        /**
         * @brief Vérifie si la pièce est blanche.
         * @return true si la pièce est blanche, false sinon.
         */
        virtual bool isWhite() = 0;
        
        /**
         * @brief Récupère les mouvements possibles de la pièce.
         * @return Un vecteur de paires d'entiers représentant les coordonnées des cases vers lesquelles la pièce peut se déplacer.
         */
        virtual std::unordered_set<std::pair<int,int>, pair_hash> getPossibleMoves(Board* board) = 0;

        /**
         * @brief Récupère la position de la pièce.
         * @return Une paire d'entiers représentant les coordonnées (ligne, colonne) de la position de la pièce.
         */
        virtual std::pair<int,int> getPosition() = 0;

        /**
         * @brief Définit la position de la pièce.
         * @param x La colonne de la position de la pièce.
         * @param y La ligne de la position de la pièce.
         */
        virtual void setPosition(int x, int y) = 0;

        /**
         * @brief Clone la pièce.
         * @return Un pointeur vers une copie de la pièce.
         */
        virtual Piece* clone() = 0;

        /**
         * @brief Vérifie si la pièce peut être transformée.
         * @return true si la pièce peut être transformée, false sinon.
         */
        virtual bool isSwitchable() = 0;

        /**
         * @brief Déplace la pièce.
         */
        virtual void move() = 0;
         
        /**
         * @brief Vérifie si la pièce a déjà bougé.
         * @return true si la pièce a déjà bougé, false sinon.
         */
        virtual bool hasMoved() = 0;

        /**
         * @brief Vérifie si la pièce peut roquer.
         * @return true si la pièce peut roquer, false sinon.
         */
        virtual bool canCastle() { return false; }

        virtual bool isKing() = 0;

        virtual bool isRook() = 0;

        virtual bool isPawn() = 0;

        virtual bool isKnight() = 0;

        virtual bool isBishop() = 0;

        virtual bool isQueen() = 0;

        virtual bool isPassedPawn(Board* board) = 0;

        virtual bool controlsCenter() = 0;

        virtual int getPositionalValue(Board* board) = 0;
};

/**
 * @brief La classe AllPieces représente une pièce d'échecs générique.
 * 
 * Cette classe hérite de la classe Piece et implémente les fonctionnalités communes à toutes les pièces.
 * Chaque instance de la classe AllPieces possède une couleur (blanc ou noir) et une position sur l'échiquier.
 */
class AllPieces : public Piece {
    protected:
        bool white;
        bool moved = false;
        std::pair<int,int> position = std::make_pair(-1,-1);
    public:
        /**
         * @brief Constructeur de la classe AllPieces.
         * 
         * @param isWhite Un booléen indiquant si la pièce est de couleur blanche.
         */
        AllPieces(bool isWhite);

        /**
         * @brief Constructeur de la classe AllPieces.
         * 
         * @param isWhite Un booléen indiquant si la pièce est de couleur blanche.
         * @param x La colonne de la position de la pièce.
         * @param y La ligne de la position de la pièce.
         */
        AllPieces(bool isWhite, int x, int y);

        /**
         * @brief Vérifie si la pièce est de couleur blanche.
         * 
         * @return true si la pièce est de couleur blanche, false sinon.
         */
        bool isWhite() override;

        /**
         * @brief Récupère la position actuelle de la pièce sur l'échiquier.
         * 
         * @return Une paire d'entiers représentant les coordonnées (ligne, colonne) de la position de la pièce.
         */
        std::pair<int,int> getPosition();

        /**
         * @brief Définit la position de la pièce sur l'échiquier.
         * 
         * @param x La colonne de la position de la pièce.
         * @param y La ligne de la position de la pièce.
         */
        void setPosition(int x, int y) override;

        /**
         * @brief Vérifie si la pièce peut être transformée.
         * 
         * @return true si la pièce peut être transformée, false sinon.
         */
        bool isSwitchable() override;
        
        /**
         * @brief Vérifie si la pièce a déjà bougé.
         * 
         * @return true si la pièce a déjà bougé, false sinon.
         */
        bool hasMoved();

        /**
         * @brief "Déplace" la pièce.
         */
        void move();

        /**
         * @brief Vérifie si la pièce est un roi.
         * 
         * @return true si la pièce est un roi, false sinon.
         */
        bool isKing();

        /**
         * @brief Vérifie si la pièce est une tour.
         * 
         * @return true si la pièce est une tour, false sinon.
         */
        bool isRook();

        /**
         * @brief Vérifie si la pièce est un pion.
         * 
         * @return true si la pièce est un pion, false sinon.
         */
        bool isPawn();

        /**
         * @brief Vérifie si la pièce est un cavalier.
         * 
         * @return true si la pièce est un cavalier, false sinon.
         */
        bool isKnight();

        /**
         * @brief Vérifie si la pièce est un fou.
         * 
         * @return true si la pièce est un fou, false sinon.
         */
        bool isBishop();

        /**
         * @brief Vérifie si la pièce est une reine.
         * 
         * @return true si la pièce est une reine, false sinon.
         */
        bool isQueen();

        /**
         * @brief Vérifie si la pièce est un pion passé.
         * 
         * @param board Le plateau de jeu.
         * @return true si la pièce est un pion passé, false sinon.
         */
        bool isPassedPawn(Board* board) override;

        /**
         * @brief Vérifie si la pièce contrôle le centre de l'échiquier.
         * 
         * @return true si la pièce contrôle le centre de l'échiquier, false sinon.
         */
        bool controlsCenter() override;
};

/**
 * @brief Classe représentant un pion dans le jeu d'échecs.
 * 
 * Cette classe hérite de la classe AllPieces et représente un pion.
 * Un pion peut se déplacer vers l'avant d'une case et capturer les pièces ennemies en diagonale.
 */
class Pawn : public AllPieces {
    public:
        /**
         * @brief Constructeur de la classe Pawn.
         * 
         * @param isWhite Booléen indiquant si le pion est blanc ou noir.
         */
        Pawn(bool isWhite);

        /**
         * @brief Constructeur de la classe Pawn.
         * 
         * @param isWhite Booléen indiquant si le pion est blanc ou noir.
         * @param x La colonne de la position du pion.
         * @param y La ligne de la position du pion.
         */
        Pawn(bool isWhite, int x, int y);

        /**
         * @brief Destructeur de la classe Pawn.
         */
        ~Pawn();

        /**
         * @brief Fonction pour obtenir le symbole du pion.
         * 
         * @return Un caractère représentant le symbole du pion.
         */
        char getSymbol() override;

        /**
         * @brief Fonction pour obtenir les mouvements possibles du pion.
         * 
         * @return Un vecteur de paires d'entiers représentant les coordonnées des cases vers lesquelles le pion peut se déplacer.
         */
        std::unordered_set<std::pair<int,int>, pair_hash> getPossibleMoves(Board* board) override;

        /**
         * @brief Fonction pour cloner un pion.
         * 
         * @return Un pointeur vers une copie du pion.
         */
        Pawn* clone();

        bool isSwitchable() override;

        int getPositionalValue(Board* board) override;
};

/**
 * @brief Classe représentant une tour dans le jeu d'échecs.
 */
class Rook : public AllPieces {
    public:
        /**
         * @brief Constructeur de la classe Rook.
         * @param isWhite Booléen indiquant si la tour est blanche ou non.
         */
        Rook(bool isWhite);

        /**
         * @brief Constructeur de la classe Rook.
         * @param isWhite Booléen indiquant si la tour est blanche ou non.
         * @param x La colonne de la position de la tour.
         * @param y La ligne de la position de la tour.
         */
        Rook(bool isWhite, int x, int y);

        /**
         * @brief Destructeur de la classe Rook.
         */
        ~Rook();

        /**
         * @brief Fonction pour obtenir le symbole de la tour.
         * @return Un caractère représentant le symbole de la tour.
         */
        char getSymbol() override;

        /**
         * @brief Fonction pour obtenir les mouvements possibles de la tour.
         * @return Un vecteur de paires d'entiers représentant les coordonnées des cases accessibles.
         */
        std::unordered_set<std::pair<int,int>, pair_hash> getPossibleMoves(Board* board) override;

        /**
         * @brief Fonction pour cloner une tour.
         * @return Un pointeur vers une copie de la tour.
         */
        Rook* clone();

        /**
         * @brief Fonction pour vérifier si la tour peut roquer.
         * @return true si la tour peut roquer, false sinon.
         */
        bool canCastle() override;

        int getPositionalValue(Board* board) override;
};

/**
 * @brief Classe représentant un Cavalier dans le jeu d'échecs.
 * 
 * Cette classe hérite de la classe AllPieces et représente un Cavalier.
 * Un Cavalier peut se déplacer en L, c'est-à-dire de deux cases dans une direction
 * et une case dans une direction perpendiculaire.
 */
class Knight : public AllPieces {
    public:
        /**
         * @brief Constructeur de la classe Knight.
         * 
         * @param isWhite Booléen indiquant si le Cavalier est blanc ou noir.
         */
        Knight(bool isWhite);

        /**
         * @brief Constructeur de la classe Knight.
         * 
         * @param isWhite Booléen indiquant si le Cavalier est blanc ou noir.
         * @param x La colonne de la position du Cavalier.
         * @param y La ligne de la position du Cavalier.
         */
        Knight(bool isWhite, int x, int y);

        /**
         * @brief Destructeur de la classe Knight.
         */
        ~Knight();

        /**
         * @brief Fonction pour obtenir le symbole du Cavalier.
         * 
         * @return Un caractère représentant le symbole du Cavalier.
         */
        char getSymbol() override;

        /**
         * @brief Fonction pour obtenir les mouvements possibles du Cavalier.
         * 
         * @return Un vecteur de paires d'entiers représentant les coordonnées des cases
         *         vers lesquelles le Cavalier peut se déplacer.
         */
        std::unordered_set<std::pair<int,int>, pair_hash> getPossibleMoves(Board* board) override;

        /**
         * @brief Fonction pour cloner un Cavalier.
         * 
         * @return Un pointeur vers une copie du Cavalier.
         */
        Knight* clone();

        int getPositionalValue(Board* board) override;
};

/**
 * @brief Classe représentant un fou dans le jeu d'échecs.
 * 
 * Cette classe hérite de la classe AllPieces et représente un fou dans le jeu d'échecs.
 * Un fou peut se déplacer en diagonale sur l'échiquier.
 */
class Bishop : public AllPieces {
    public:
        /**
         * @brief Constructeur de la classe Bishop.
         * 
         * @param isWhite Booléen indiquant si le fou est blanc ou noir.
         */
        Bishop(bool isWhite);

        /**
         * @brief Constructeur de la classe Bishop.
         * 
         * @param isWhite Booléen indiquant si le fou est blanc ou noir.
         * @param x La colonne de la position du fou.
         * @param y La ligne de la position du fou.
         */
        Bishop(bool isWhite, int x, int y);

        /**
         * @brief Destructeur de la classe Bishop.
         */
        ~Bishop();

        /**
         * @brief Fonction pour obtenir le symbole du fou.
         * 
         * @return Un caractère représentant le symbole du fou.
         */
        char getSymbol() override;

        /**
         * @brief Obtient les mouvements possibles pour le fou.
         * 
         * @return Un vecteur de paires d'entiers représentant les coordonnées des cases vers lesquelles le fou peut se déplacer.
         */
        std::unordered_set<std::pair<int,int>, pair_hash> getPossibleMoves(Board* board) override;

        /**
         * @brief Fonction pour cloner un fou.
         * 
         * @return Un pointeur vers une copie du fou.
         */
        Bishop* clone();

        int getPositionalValue(Board* board) override;
};

/**
 * @brief Classe représentant une reine dans le jeu d'échecs.
 * 
 * La classe Queen hérite de la classe AllPieces et représente une pièce de type reine.
 * Elle possède une méthode pour obtenir les mouvements valides de la reine.
 */
class Queen : public AllPieces {
    public:
        /**
         * @brief Constructeur de la classe Queen.
         * 
         * @param isWhite Booléen indiquant si la reine est blanche ou noire.
         */
        Queen(bool isWhite);

        /**
         * @brief Constructeur de la classe Queen.
         * 
         * @param isWhite Booléen indiquant si la reine est blanche ou noire.
         * @param x La colonne de la position de la reine.
         * @param y La ligne de la position de la reine.
         */
        Queen(bool isWhite, int x, int y);

        /**
         * @brief Destructeur de la classe Queen.
         */
        ~Queen();

        /**
         * @brief Méthode pour obtenir le symbole de la reine.
         * 
         * @return Un caractère représentant le symbole de la reine.
         */
        char getSymbol() override;

        /**
         * @brief Méthode pour obtenir les mouvements valides de la reine.
         * 
         * @return Un vecteur de paires d'entiers représentant les coordonnées des cases vers lesquelles la reine peut se déplacer.
         */
        std::unordered_set<std::pair<int,int>, pair_hash> getPossibleMoves(Board* board) override;

        /**
         * @brief Méthode pour cloner une reine.
         * 
         * @return Un pointeur vers une copie de la reine.
         */
        Queen* clone();

        int getPositionalValue(Board* board) override;
};

/**
 * @brief Classe représentant le roi dans le jeu d'échecs.
 * 
 * Cette classe hérite de la classe AllPieces et représente le roi dans le jeu d'échecs.
 * Le roi peut se déplacer dans toutes les directions d'une case à la fois.
 */
class King : public AllPieces {
    public:
        /**
         * @brief Constructeur de la classe King.
         * 
         * @param isWhite Booléen indiquant si le roi est blanc ou non.
         */
        King(bool isWhite);

        /**
         * @brief Constructeur de la classe King.
         * 
         * @param isWhite Booléen indiquant si le roi est blanc ou non.
         * @param x La colonne de la position du roi.
         * @param y La ligne de la position du roi.
         */
        King(bool isWhite, int x, int y);

        /**
         * @brief Destructeur de la classe King.
         */
        ~King();

        /**
         * @brief Fonction pour obtenir le symbole du roi.
         * 
         * @return Un caractère représentant le symbole du roi.
         */
        char getSymbol() override;

        /**
         * @brief Fonction pour obtenir les mouvements possibles du roi.
         * 
         * Cette fonction retourne un vecteur de paires d'entiers représentant les coordonnées des cases
         * vers lesquelles le roi peut se déplacer.
         * 
         * @return Un vecteur de paires d'entiers représentant les coordonnées des cases vers lesquelles le roi peut se déplacer.
         */
        std::unordered_set<std::pair<int,int>, pair_hash> getPossibleMoves(Board* board) override;

        /**
         * @brief Fonction pour cloner un roi.
         * 
         * @return Un pointeur vers une copie du roi.
         */
        King* clone();

        /**
         * @brief Fonction pour vérifier si le roi peut roquer.
         * 
         * @return true si le roi peut roquer, false sinon.
         */
        bool canCastle() override;

        int getPositionalValue(Board* board) override;
};

#endif