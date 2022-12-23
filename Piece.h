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
    Piece(Color color, PieceType type);

    Color getColor() const;

    PieceType getType() const;

    const LinkedList<Location *> &getLegalMoves() const;

    void setType(PieceType type);

private:
    Color color;
    PieceType type;
    LinkedList<Location *> legalMoves{};
};


#endif //CHESSGAME_PIECE_H
