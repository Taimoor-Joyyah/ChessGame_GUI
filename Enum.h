//
// Created by J-Bros on 12/17/2022.
//

#ifndef CHESSGAME_ENUM_H
#define CHESSGAME_ENUM_H

enum Key {
    UP = 56,        //NUM 8
    DOWN = 50,      //NUM 2
    LEFT = 52,      //NUM 4
    RIGHT = 54,     //NUM 6
    SELECT = 53,    //NUM 5
    ESC = 27        //ESCAPE
};

enum Color {
    WHITE,
    BLACK
};

enum PieceType {
    KING = 'K',
    QUEEN = 'Q',
    BISHOP = 'B',
    KNIGHT = 'N',
    ROOK = 'R',
    PAWN = 'W',
};

#endif //CHESSGAME_ENUM_H
