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
#include "Piece.h"
#include "Rules.h"

using namespace std;

class Chess {
public:
    Chess();
    void startGame();

protected:
    static Frame frame;
    Piece *pieces[8][8]{};

    static Menu pauseMenu;
    static Menu expertMenu;
    static Menu quitMenu;

    bool expertMode = false;
    Location currentCell{3, 3};
    Color currentPlayer = WHITE;
    Location selectedCell{-1, -1};
    LinkedList<Location *> possibles{};
    time_t timePassed = 0;

    void setupBoard();
    void expert_menu();
    bool pause_menu(time_t &startTime);
    void move();
    void move(const Location &from, const Location &to);
    void changePlayer();

    static string timeToString(int seconds);
    void resetCellFrame(const Location &cell);
    void setupBoardFrame();
    void setupPiecesFrame();
    void updatePlayerFrame();
    void updateTimeFrame();
    void updateCurrentCellFrame();
    void updateSelectedCellFrame();
    void updatePossibleCellFrame();
    void updatePieceFrame(const Location &cell);
};


#endif //CHESSGAME_CHESS_H
