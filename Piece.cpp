//
// Created by J-Bros on 12/18/2022.
//

#include "Piece.h"

Piece::Piece(P_Color color, PieceType type) : color(color), type(type) {}

P_Color Piece::getColor() const {
    return color;
}

PieceType Piece::getType() const {
    return type;
}

void Piece::setType(PieceType type) {
    this->type = type;
}

LinkedList<struct Location *> &Piece::getLegalMoves() {
    return legalMoves;
}

