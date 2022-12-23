//
// Created by J-Bros on 12/18/2022.
//

#include "Piece.h"

Piece::Piece(Color color, PieceType type) : color(color), type(type) {}

Color Piece::getColor() const {
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

void Piece::setLegalMoves(LinkedList<struct Location *> &legalMoves) {
    this->legalMoves.clear();
    this->legalMoves.merge(legalMoves);
}
