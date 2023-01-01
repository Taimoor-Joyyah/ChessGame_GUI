//
// Created by J-Bros on 12/17/2022.
//

#ifndef CHESSGAME_CHESS_H
#define CHESSGAME_CHESS_H

#include <sstream>
#include "LinkedList.h"
#include "Enum.h"
#include "Location.h"
#include "Menu.h"
#include "Popup.h"
#include "Piece.h"
#include "Ending.h"

using namespace std;

class Chess {
public:
    Chess(bool isContinue);

    ~Chess();

    void startGame();

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
    P_Color currentPlayer = P_WHITE;
    Location selectedCell{-1};
    time_t timePassed = 0;
    int session = 0;
    bool isWhite = true;

    int whitePoints = 0;
    int blackPoints = 0;

    Location enPassantTo{-1};
    int enPassantSession{};

    bool castlingRule[6]{};// wlR, wK, wrR, blR, bK, brR // hasMoved

    bool loaded = true;

    static string timeToString(int seconds);

private:
    Location *getKingLocation(Piece *pieceList[8][8], P_Color player);

    P_Color getOpponent();

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

    bool selectCell();

    bool changePlayer();

    void save();

    bool load();

    void deleteSave();

    void binaryWriteInt(ofstream &stream, int &data) const;

    void binaryReadInt(ifstream &stream, int &data) const;

    // Rules Functions

    bool isOnlyKing(P_Color player);

    bool isCheckOn(Piece *pieceList[8][8], Location *cell);

    void getMoves(Piece *pieceList[8][8], const Location &cell, LinkedList<Location *> &moves, bool isSimulating);

    void addStraight(Piece *pieceList[8][8], const Location &cell, LinkedList<Location *> &moves, bool isSimulating);

    void addDiagonal(Piece *pieceList[8][8], const Location &cell, LinkedList<Location *> &moves, bool isSimulating);

    bool addLocation(Piece *pieceList[8][8], int rank, int file,
                     const Piece *currentPiece, LinkedList<Location *> &moves, bool isSimulating, bool couldLeap);
};


#endif //CHESSGAME_CHESS_H
