//
// Created by J-Bros on 12/18/2022.
//

#ifndef CHESSGAME_PIECE_H
#define CHESSGAME_PIECE_H

#include "Enum.h"
#include "Location.h"
#include "LinkedList.h"

class Piece {
public:
    Piece(P_Color color, PieceType type);

    P_Color getColor() const;

    PieceType getType() const;

    LinkedList<struct Location *> &getLegalMoves();

    void setLegalMoves(LinkedList<struct Location *> &legalMoves);

    void setType(PieceType type);

private:
    P_Color color;
    PieceType type;
    LinkedList<Location *> legalMoves{};
};


#endif //CHESSGAME_PIECE_H
