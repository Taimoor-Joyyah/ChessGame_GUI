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
    time_t timePassed = 0;
    int session = 0;
    bool isWhite = true;

    int whitePoints = 0;
    int blackPoints = 0;

    Location enPassantTo{-1, -1};
    int enPassantSession{};

    bool castlingRule[6]{};// wlR, wK, wrR, blR, bK, brR // hasMoved

    Location *getKingLocation(Color player);

    Color getOpponent();

    Piece *getPiece(const Location &cell);

    void setPiece(const Location &cell, Piece *piece);

    void setupBoard();

    void expert_menu();

    bool pause_menu(time_t &startTime);

    bool updateStatus();

    void move(Location &selected);

    void move(Location &from, Location &to);

    bool simulate();

    void addIfCastling();

    void evaluateCastling(const Location &cell);

    void castling(const Location &from, const Location &to);

    void promotion(const Location &to, Piece *fromPiece);

    void evaluateEnPassant(const Location &from, const Location &to);

    void enPassantMove(const Location &to);

    void evaluatePoints(const Piece *toPiece);

    void clearSelected();

    bool selectCell();

    bool changePlayer();

    // Rules Functions

    bool isOnlyKing(Color player);

    bool isCheckOn(Location *cell);

    void getMoves(const Location &cell, LinkedList<Location *> &moves, bool isSimulating);

    void addStraight(const Location &cell, LinkedList<Location *> &possibles, bool isSimulating);

    void addDiagonal(const Location &cell, LinkedList<Location *> &possibles, bool isSimulating);

    bool
    addLocation(int rank, int file, const Piece *currentPiece, LinkedList<Location *> &possibles, bool isSimulating,
                bool couldLeap = false);

    // Frame Functions

    static string timeToString(int seconds);

    void resetCellFrame(const Location &cell);

    void setupBoardFrame();

    void setupPiecesFrame();

    void updateFrame();

    void updatePlayerFrame();

    void updateTimeFrame();

    void updatePointsFrame();

    void updateCurrentCellFrame();

    void updateSelectedCellFrame();

    void updatePieceFrame(const Location &cell);

    void updateLegalMovesFrame();

    void clearLegalMovesFrame();
};


#endif //CHESSGAME_CHESS_H
