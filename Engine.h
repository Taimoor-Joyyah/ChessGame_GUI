//
// Created by mtaimoor on 9/8/2023.
//

#ifndef CHESSGAME_GUI_ENGINE_H
#define CHESSGAME_GUI_ENGINE_H

#include "Piece.h"

struct Move {
    Piece *piece;
    Location location;
};

struct GameState {
    Piece *pieces[8][8];
    P_Color *player;
};

class Engine {
public:
    static void setEngine(Piece ***pieceList, P_Color *currentPlayer, int power);
    static void setPower(int power);

    static Move evaluateBestMove();

private:
    static Piece ***pieceList;
    static P_Color *currentPlayer;
    static int power;
};


#endif //CHESSGAME_GUI_ENGINE_H
