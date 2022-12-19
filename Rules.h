//
// Created by J-Bros on 12/18/2022.
//

#ifndef CHESSGAME_RULES_H
#define CHESSGAME_RULES_H

#include "Piece.h"
#include "LinkedList.h"

class Rules {
public:
    static void possibleMoves(const Location &cell, Piece *pieces[8][8], LinkedList<Location *> &possibles);

private:
    static void addStraight(const Location &cell, const Piece *currentPiece,
                            Piece *pieces[8][8], LinkedList<Location *> &possibles);

    static void addDiagonal(const Location &cell, const Piece *currentPiece,
                            Piece *pieces[8][8], LinkedList<Location *> &possibles);

    static bool addLocation(int rank, int file, const Piece *currentPiece,
                            Piece *pieces[8][8], LinkedList<Location *> &possibles, bool couldLeap = false);
};

#endif //CHESSGAME_RULES_H
