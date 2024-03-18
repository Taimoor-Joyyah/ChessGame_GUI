//
// Created by mtaimoor on 9/8/2023.
//

#ifndef CHESSGAME_GUI_ENGINE_H
#define CHESSGAME_GUI_ENGINE_H

#include <vector>
#include "Piece.h"
#include "Chess.h"

struct Move {
    Location fromLocation;
    Location toLocation;
};

struct GameState {
    Piece *pieces[8][8]{};
    P_Color player{};

    GameState(Piece *pieces[8][8], P_Color player);

    std::vector<Location> getPieces(P_Color player);
};

class Engine {
public:

    static Move evaluateBestMove(Piece *pieces[8][8], P_Color player, int power);

private:
    static int minimax(GameState gameState, int depth, int alpha, int beta, bool isMaximizingPlayer);

    static GameState movePiece(GameState *gameState, Move move);

    static std::vector<Move> getAllPossibleMoves(GameState &gameState);

    static int evaluateBoard(GameState &gameState);

    static bool isGameOver(GameState &state);
};

#endif //CHESSGAME_GUI_ENGINE_H
