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

using namespace std;

class Chess {
public:
    Chess();

    void startGame();

private:

    static Frame frame;
    Piece *pieces[8][8]{};
    static Menu pauseMenu;

    static Menu expertMenu;
    static Menu quitMenu;
    static Menu promotionMenu;
    bool expertMode = false;

    Location currentCell{3, 3};
    Color currentPlayer = WHITE;
    Location selectedCell{-1, -1};
    LinkedList<Location *> selectedPossibles{};
    time_t timePassed = 0;
    int session = 0;
    int whitePoints = 0;

    int blackPoints = 0;
    Location enPassantTo{-1, -1};

    int enPassantSession{};

    void setupBoard();

    void expert_menu();

    bool pause_menu(time_t &startTime);

    void move();

    void move(const Location &from, const Location &to);

    void changePlayer();

    // Rules Functions

    void possibleMoves(const Location &cell, LinkedList<Location *> &possibles);

    void addStraight(const Location &cell, LinkedList<Location *> &possibles);

    void addDiagonal(const Location &cell, LinkedList<Location *> &possibles);

    bool addLocation(int rank, int file,const Piece *currentPiece, LinkedList<Location *> &possibles, bool couldLeap = false);

    // Frame Functions

    static string timeToString(int seconds);

    void resetCellFrame(const Location &cell);

    void setupBoardFrame();

    void setupPiecesFrame();

    void updatePlayerFrame();

    void updateTimeFrame();

    void updatePointsFrame();

    void updateCurrentCellFrame();

    void updateSelectedCellFrame();

    void updatePossibleCellFrame();

    void updatePieceFrame(const Location &cell);
};


#endif //CHESSGAME_CHESS_H
