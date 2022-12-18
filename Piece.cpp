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
