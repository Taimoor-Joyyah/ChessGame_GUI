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
};


#endif //CHESSGAME_RULES_H
