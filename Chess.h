//
// Created by J-Bros on 12/17/2022.
//

#ifndef CHESSGAME_CHESS_H
#define CHESSGAME_CHESS_H

#include <sstream>
#include "LinkedList.h"
#include "Frame.h"
#include "Enum.h"
#include "Location.h"
#include "Menu.h"

using namespace std;

class Chess {
public:
    Chess();
    void startGame();

protected:
    struct Piece;
    static Frame frame;

    static Menu pauseMenu;
    static Menu expertMenu;
    static Menu quitMenu;

    bool expertMode = false;
    Location currentCell{3, 3};
    Color currentPlayer = WHITE;
    LinkedList<Piece *> pieces{};
    Location selectedCell{-1, -1};
    time_t timePassed = 0;

    void setupBoard();
    Piece *getPiece(const Location &cell);

    static string timeToString(int seconds);

    void resetCellFrame(const Location &cell);
    void setupBoardFrame();
    void setupPiecesFrame();
    void updatePlayerFrame();
    void updateTimeFrame();
    void updateCurrentCellFrame();
    void updateSelectedCellFrame();
};


#endif //CHESSGAME_CHESS_H
