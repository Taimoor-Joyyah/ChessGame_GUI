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
#include "Popup.h"
#include "Piece.h"
#include "Input.h"
#include "Ending.h"

using namespace std;

class Chess {
public:
    Chess();

    ~Chess();

    void startGame();

private:

    static Frame frame;
    Piece *pieces[8][8]{};
    static Menu pauseMenu;

    static Menu expertMenu;
    static Menu quitMenu;
    static Menu promotionMenu;
    static Popup help;
    static Ending checkmate;
    static Ending stalemate;
    static Ending deadPositions;
    bool expertMode = false;

    Location currentCell{3, 3};
    Color currentPlayer = WHITE;
    Location selectedCell{-1, -1};
    LinkedList<Location *> selectedLegalMoves{};
    time_t timePassed = 0;
    int session = 0;

    int whitePoints = 0;
    int blackPoints = 0;

    Location enPassantTo{-1, -1};
    int enPassantSession{};

    bool castlingRule[6]{};// wlR, wK, wrR, blR, bK, brR // hasMoved

    LinkedList<Location *> opponentLegalMoves{};
    LinkedList<Location *> playerLegalMoves{};
    Location *getKingLocation(Color player);

    int pieceCheckCount = 0;

    int debugState = 0;

    void setupBoard();

    void expert_menu();

    bool pause_menu(time_t &startTime);

    void move();

    void move(const Location &from, const Location &to);

    void clearSelected();

    bool changePlayer();

    // Rules Functions

    bool isOnlyKing(Color player);

    bool isCheck(Color player);

    void getAllLegalMoves(Color player, LinkedList<Location *> &allMoves, LinkedList<Location *> &checkMoves,
                          bool isSimulating);

    void possibleMoves(const Location &cell, LinkedList<Location *> &possibles, LinkedList<Location *> &legalMoves,
                       bool isSimulating = false);

    void addStraight(const Location &cell, LinkedList<Location *> &possibles, bool isSimulating);

    void addDiagonal(const Location &cell, LinkedList<Location *> &possibles, bool isSimulating);

    bool addLocation(int rank, int file, const Piece *currentPiece, LinkedList<Location *> &possibles, bool isSimulating,
                     bool couldLeap = false);

    bool updateStatus();

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

    void updatePossibleCellFrame(LinkedList<Location *> &listMoves);

    void updatePieceFrame(const Location &cell);

    void updateDebug();

    void clearPossibles(LinkedList<Location *> &allMoves);

    void cleanPossiblesFrame(LinkedList<Location *> &listMoves);
};


#endif //CHESSGAME_CHESS_H
