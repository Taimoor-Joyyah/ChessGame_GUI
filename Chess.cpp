//
// Created by J-Bros on 12/17/2022.
//

#include <cmath>
#include <ctime>
#include <fstream>
#include <unistd.h>
#include "Chess.h"
#include "ChessWindow.h"
#include "raylib.h"
#include "Engine.h"

#define DEBUG false

Chess::Chess(bool isContinue): Chess(isContinue, 0, P_HUMAN, P_CPU, 0, 0) {
}

Chess::Chess(bool isContinue, int mode, P_Type whiteType, P_Type blackType, int whitePower, int blackPower)
    : mode(mode), whiteType(whiteType), blackType(blackType), whitePower(whitePower), blackPower(blackPower) {
    if (isContinue) {
        loaded = load();
    } else {
        deleteSave();
        setupBoard();
    }
}

Chess::~Chess() {
    for (auto &piece: pieces)
        for (auto &x: piece)
            delete x;
}

Menu Chess::quitMenu{
    "QUIT TO MENU",
    new string[]{
        "No",
        "Yes"
    },
    2,
    true
};

Menu Chess::pauseMenu{
    "PAUSE MENU",
    new string[]{
        "Continue",
        "Expert Mode",
        "Save",
        "Help",
        "Quit"
    },
    5,
    true
};

Menu Chess::promotionMenu{
    "PROMOTION",
    new string[]{
        "Queen",
        "Rook",
        "Bishop",
        "Knight"
    },
    4,
    false
};

Menu Chess::expertMenu{
    "EXPERT MODE",
    new string[]{
        "No",
        "Yes"
    },
    2,
    true
};

Popup Chess::help{"../Help.txt"};

Ending Chess::checkmate{nullptr};
Ending Chess::stalemate{new string[]{"DRAW", "STALEMATE"}};
Ending Chess::deadPositions{new string[]{"DRAW", "DEAD POSITIONS"}};

void Chess::setupBoard() {
    setPiece(pieces, {0, 0}, new Piece{P_BLACK, ROOK});
    setPiece(pieces, {0, 1}, new Piece{P_BLACK, KNIGHT});
    setPiece(pieces, {0, 2}, new Piece{P_BLACK, BISHOP});
    setPiece(pieces, {0, 3}, new Piece{P_BLACK, QUEEN});
    setPiece(pieces, {0, 4}, new Piece{P_BLACK, KING});
    setPiece(pieces, {0, 5}, new Piece{P_BLACK, BISHOP});
    setPiece(pieces, {0, 6}, new Piece{P_BLACK, KNIGHT});
    setPiece(pieces, {0, 7}, new Piece{P_BLACK, ROOK});

    setPiece(pieces, {7, 0}, new Piece{P_WHITE, ROOK});
    setPiece(pieces, {7, 1}, new Piece{P_WHITE, KNIGHT});
    setPiece(pieces, {7, 2}, new Piece{P_WHITE, BISHOP});
    setPiece(pieces, {7, 3}, new Piece{P_WHITE, QUEEN});
    setPiece(pieces, {7, 4}, new Piece{P_WHITE, KING});
    setPiece(pieces, {7, 5}, new Piece{P_WHITE, BISHOP});
    setPiece(pieces, {7, 6}, new Piece{P_WHITE, KNIGHT});
    setPiece(pieces, {7, 7}, new Piece{P_WHITE, ROOK});

    for (int i = 0; i < 8; ++i) {
        setPiece(pieces, {1, i}, new Piece{P_BLACK, PAWN});
        setPiece(pieces, {6, i}, new Piece{P_WHITE, PAWN});
    }
}

void Chess::startGame() {
    if (!loaded)
        return;
    updateStatus();

    ChessWindow::game = this;

    if (mode == 1 &&
        (currentPlayer == P_WHITE && whiteType == P_CPU ||
         currentPlayer == P_BLACK && blackType == P_CPU) &&
        !cpuMove()) {
        if (!DEBUG)
            usleep(1000000);
        ChessWindow::game = nullptr;
        return;
    }

    if (mode == 2) {
        while (true) {
            if (!DEBUG)
                usleep(2000000);
            if (!cpuMove()) {
                ChessWindow::game = nullptr;
                return;
            }
        }
    }

    time_t startTime = time(nullptr) - timePassed;
    Vector2 mousePos;
    bool mouseClicked = true;
    while (true) {
        mousePos = GetMousePosition();
        auto delta = GetMouseDelta();
        if (mousePos.y >= 64 && mousePos.y < 576 && mousePos.x >= 64 && mousePos.x < 576) {
            if (delta.x != 0 || delta.y != 0) {
                int rank = mousePos.y / 64 - 1;
                int file = mousePos.x / 64 - 1;
                currentCell.set(rank, file);
            }
            if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
                mouseClicked = false;
            else if (!mouseClicked && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (!selectCell()) {
                    ChessWindow::game = nullptr;
                    return;
                }
                mouseClicked = true;
            }
        }


        switch (GetKeyPressed()) {
            case KEY_KP_8:
                currentCell.rank = currentCell.rank == 0 ? 7 : currentCell.rank - 1;
                break;
            case KEY_KP_2:
                currentCell.rank = (currentCell.rank + 1) % 8;
                break;
            case KEY_KP_4:
                currentCell.file = currentCell.file == 0 ? 7 : currentCell.file - 1;
                break;
            case KEY_KP_6:
                currentCell.file = (currentCell.file + 1) % 8;
                break;
            case KEY_KP_5:
                if (!selectCell()) {
                    ChessWindow::game = nullptr;
                    return;
                }
                break;
            case KEY_ESCAPE:
                if (!pause_menu(startTime)) {
                    ChessWindow::game = nullptr;
                    return;
                }
        }
        time_t currentTime = time(nullptr) - startTime;
        if (timePassed != currentTime) {
            timePassed = currentTime;
        }
    }
}

bool Chess::cpuMove() {
    Move engineMove = Engine::evaluateBestMove(pieces, currentPlayer, blackPower);
    move(engineMove.fromLocation, engineMove.toLocation);
    if (!changePlayer()) {
        deleteSave();
        return false;
    }
    return true;
}

bool Chess::selectCell() {
    if (currentCell == selectedCell) {
        selectedCell.set(-1, -1);
        return true;
    }
    Piece *currentPiece = getPiece(pieces, currentCell);
    Piece *selectedPiece = selectedCell.rank == -1 ? nullptr : getPiece(pieces, selectedCell);

    if (selectedPiece != nullptr && selectedPiece->getLegalMoves().contains(&currentCell)) {
        Location temp{selectedCell.rank, selectedCell.file};
        selectedCell.set(-1, -1);
        move(temp);
        if (!changePlayer()) {
            deleteSave();
            return false;
        }

        if (mode == 1 &&
            (currentPlayer == P_WHITE && whiteType == P_CPU ||
             currentPlayer == P_BLACK && blackType == P_CPU) &&
            !cpuMove())
            return false;
    } else if (currentPiece != nullptr && currentPiece->getColor() == currentPlayer) {
        if (!currentPiece->getLegalMoves().isEmpty())
            selectedCell.set(currentCell.rank, currentCell.file);
    } else if (selectedPiece != nullptr && !selectedPiece->getLegalMoves().isEmpty())
        selectedCell.set(-1, -1);
    return true;
}

bool Chess::changePlayer() {
    currentPlayer = isWhite ? P_BLACK : P_WHITE;
    isWhite = !isWhite;
    ++session;
    return updateStatus();
}

bool Chess::updateStatus() {
    bool hasLegalMove = simulate();
    bool isCheck = isCheckOn(pieces, getKingLocation(pieces, currentPlayer), isWhite, &session, &enPassantSession,
                             &enPassantTo);
    if (!isCheck)
        addIfCastling();
    if (!hasLegalMove) {
        if (isCheck) {
            checkmate.setText(new string[]{"CHECKMATE", isWhite ? "BLACK WON" : "WHITE WON"});
            checkmate.pop();
            return false;
        }

        stalemate.pop();
        return false;
    }

    for (auto &rank: pieces)
        for (auto piece: rank)
            if (piece != nullptr && piece->getType() != KING) {
                return true;
            }
    deadPositions.pop();
    return false;
}

void Chess::addIfCastling() {
    int castlingKingIndex = isWhite ? 4 : 1;
    if (!castlingRule[castlingKingIndex]) {
        int king_rank = isWhite ? 7 : 0;
        for (int i = -1; i <= 1; i += 2) {
            if (!castlingRule[castlingKingIndex + i] &&
                (i == 1
                     ? getPiece(pieces, {king_rank, 6}) == nullptr
                     : getPiece(pieces, {king_rank, 1}) == getPiece(pieces, {king_rank, 2}))) {
                Piece *king = getPiece(pieces, *getKingLocation(pieces, currentPlayer));
                if (king->getLegalMoves().contains(new Location{king_rank, 4 + i}) &&
                    !isCheckOn(pieces, new Location{king_rank, 4 + i * 2}, isWhite, &session, &enPassantSession,
                               &enPassantTo)) {
                    king->getLegalMoves().insert(new Location{king_rank, 4 + i * 2});
                }
            }
        }
    }
}

bool Chess::simulate() {
    bool hasLegalMove = false;

    Piece *tempPieces[8][8];
    for (int rank = 0; rank < 8; ++rank)
        for (int file = 0; file < 8; ++file)
            tempPieces[rank][file] = pieces[rank][file];

    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            Piece *piece = tempPieces[rank][file];
            if (piece != nullptr && piece->getColor() == currentPlayer) {
                LinkedList<Location *> &moves = piece->getLegalMoves();
                moves.clear();
                getMoves(tempPieces, {rank, file}, moves, false, &session, &enPassantSession, &enPassantTo);
                tempPieces[rank][file] = nullptr;
                moves.iteratorReset();
                while (!moves.isIteratorEnd()) {
                    Location *move = moves.iteratorNext();
                    Piece *temp = tempPieces[move->rank][move->file];
                    tempPieces[move->rank][move->file] = piece;
                    if (isCheckOn(tempPieces, getKingLocation(tempPieces, currentPlayer), isWhite, &session,
                                  &enPassantSession, &enPassantTo))
                        moves.remove(move);
                    tempPieces[move->rank][move->file] = temp;
                }
                tempPieces[rank][file] = piece;
                hasLegalMove = hasLegalMove || !moves.isEmpty();
            }
        }
    }
    return hasLegalMove;
}

void Chess::move(Location &selected) {
    move(selected, currentCell);
}

void Chess::move(Location &from, Location &to) {
    Piece *fromPiece = getPiece(pieces, from);
    Piece *toPiece = getPiece(pieces, to);

    castling(from, to);
    evaluateCastling(from);

    if (fromPiece->getType() == PAWN) {
        promotion(to, fromPiece);
        evaluateEnPassant(from, to);
        enPassantMove(to);
    }

    evaluatePoints(toPiece);

    ChessWindow::animation = new ChessWindow::Animation{fromPiece, 30, from, to};
    setPiece(pieces, from, nullptr);
    setPiece(pieces, to, fromPiece);
    if (!DEBUG)
        usleep(500000);

    delete toPiece;
}

void Chess::evaluatePoints(const Piece *toPiece) {
    if (toPiece != nullptr)
        switch (toPiece->getType()) {
            case QUEEN:
                (isWhite ? whitePoints : blackPoints) += 9;
                break;
            case BISHOP:
            case KNIGHT:
                (isWhite ? whitePoints : blackPoints) += 3;
                break;
            case ROOK:
                (isWhite ? whitePoints : blackPoints) += 5;
                break;
            case PAWN:
                (isWhite ? whitePoints : blackPoints) += 1;
                break;
        }
}

void Chess::enPassantMove(const Location &to) {
    if (session == enPassantSession + 1) {
        int rank = isWhite ? to.rank + 1 : to.rank - 1;
        if (enPassantTo.equals(rank, to.file)) {
            setPiece(pieces, enPassantTo, nullptr);
            enPassantTo.set(-1, -1);
            enPassantSession = -1;
            (isWhite ? whitePoints : blackPoints) += 1;
        }
    }
}

void Chess::evaluateEnPassant(const Location &from, const Location &to) {
    if (abs(from.rank - to.rank) == 2) {
        enPassantSession = session;
        enPassantTo.set(to.rank, to.file);
    }
}

void Chess::promotion(const Location &to, Piece *fromPiece) {
    if (isWhite ? to.rank == 0 : to.rank == 7) {
        if (mode != 0 &&
            (currentPlayer == P_WHITE && whiteType == P_CPU ||
             currentPlayer == P_BLACK && blackType == P_CPU)) {
            fromPiece->setType(QUEEN);
            return;
        }
        switch (promotionMenu.selectOption()) {
            case 0:
                fromPiece->setType(QUEEN);
                break;
            case 1:
                fromPiece->setType(ROOK);
                break;
            case 2:
                fromPiece->setType(BISHOP);
                break;
            case 3:
                fromPiece->setType(KNIGHT);
        }
    }
}

void Chess::castling(const Location &from, const Location &to) {
    if (getPiece(pieces, from)->getType() == KING && abs(from.file - to.file) == 2) {
        Location rook{from.rank, (to.file == 6 ? 7 : 0)};
        Location moveTo{from.rank, (to.file == 6 ? 5 : 3)};
        move(rook, moveTo);
    }
}

Piece *Chess::getPiece(Piece *pieces[8][8], const Location &cell) {
    return pieces[cell.rank][cell.file];
}

void Chess::setPiece(Piece *pieces[8][8], const Location &cell, Piece *piece) {
    pieces[cell.rank][cell.file] = piece;
}

void Chess::evaluateCastling(const Location &cell) {
    Piece *piece = getPiece(pieces, cell);
    int castlingIndex = isWhite ? 4 : 1;
    if (piece->getType() == KING)
        castlingRule[castlingIndex] = true;
    else if (piece->getType() == ROOK) {
        int direction = cell.file == 7 ? 1 : -1;
        castlingRule[castlingIndex + direction] = true;
    }
}

Location *Chess::getKingLocation(Piece *pieceList[8][8], P_Color player) {
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            Piece *piece = pieceList[rank][file];
            if (piece != nullptr && piece->getColor() == player && piece->getType() == KING)
                return new Location{rank, file};
        }
    }
    return nullptr;
}

bool Chess::pause_menu(time_t &startTime) {
    time_t delay = time(nullptr);
    int option;
    do {
        option = pauseMenu.selectOption();
        switch (option) {
            case 1:
                expert_menu();
                break;
            case 2:
                save();
                break;
            case 3:
                help.pop();
                break;
            case 4:
                if (quitMenu.selectOption() == 1)
                    return false;
        }
    } while (option != 0 && option != 2 && option != -1);
    startTime += time(nullptr) - delay;
    return true;
}

void Chess::expert_menu() {
    switch (expertMenu.selectOption()) {
        case 0:
            expertMode = false;
            break;
        case 1:
            expertMode = true;
    }
}

void Chess::save() {
    ofstream stream("prevState.sav", ios::binary | ios::out);
    if (stream.is_open()) {
        int temp = expertMode ? 1 : 0;
        binaryWriteInt(stream, temp);
        temp = isWhite ? 1 : 0;
        binaryWriteInt(stream, temp);

        binaryWriteInt(stream, mode);
        binaryWriteInt(stream, reinterpret_cast<int &>(whiteType));
        binaryWriteInt(stream, reinterpret_cast<int &>(blackType));
        binaryWriteInt(stream, whitePower);
        binaryWriteInt(stream, blackPower);
        binaryWriteInt(stream, reinterpret_cast<int &>(timePassed));
        binaryWriteInt(stream, session);
        binaryWriteInt(stream, enPassantSession);
        binaryWriteInt(stream, whitePoints);
        binaryWriteInt(stream, blackPoints);
        binaryWriteInt(stream, currentCell.rank);
        binaryWriteInt(stream, currentCell.file);
        binaryWriteInt(stream, selectedCell.rank);
        binaryWriteInt(stream, selectedCell.file);
        binaryWriteInt(stream, enPassantTo.rank);
        binaryWriteInt(stream, enPassantTo.file);
        for (bool i: castlingRule) {
            temp = i ? 1 : 0;
            binaryWriteInt(stream, temp);
        }
        for (int rank = 0; rank < 8; ++rank) {
            for (int file = 0; file < 8; ++file) {
                Piece *piece = getPiece(pieces, {rank, file});
                if (piece != nullptr) {
                    temp = piece->getColor() == P_WHITE ? 1 : 0;
                    binaryWriteInt(stream, temp);
                    temp = (int) piece->getType();
                    binaryWriteInt(stream, temp);
                } else {
                    temp = -1;
                    binaryWriteInt(stream, temp);
                    binaryWriteInt(stream, temp);
                }
            }
        }
        stream.close();
    }
}

bool Chess::load() {
    ifstream stream("prevState.sav", ios::binary | ios::in);
    if (stream.is_open()) {
        int data;
        binaryReadInt(stream, data);
        expertMode = data == 1;
        binaryReadInt(stream, data);
        currentPlayer = data == 1 ? P_WHITE : P_BLACK;
        isWhite = currentPlayer == P_WHITE;
        binaryReadInt(stream, mode);
        binaryReadInt(stream, data);
        whiteType = static_cast<P_Type>(data);
        binaryReadInt(stream, data);
        blackType = static_cast<P_Type>(data);
        binaryReadInt(stream, whitePower);
        binaryReadInt(stream, blackPower);
        binaryReadInt(stream, data);
        timePassed = data;
        binaryReadInt(stream, session);
        binaryReadInt(stream, enPassantSession);
        binaryReadInt(stream, whitePoints);
        binaryReadInt(stream, blackPoints);
        binaryReadInt(stream, currentCell.rank);
        binaryReadInt(stream, currentCell.file);
        binaryReadInt(stream, selectedCell.rank);
        binaryReadInt(stream, selectedCell.file);
        binaryReadInt(stream, enPassantTo.rank);
        binaryReadInt(stream, enPassantTo.file);

        for (bool &i: castlingRule) {
            binaryReadInt(stream, data);
            i = data == 1;
        }
        for (int rank = 0; rank < 8; ++rank) {
            for (int file = 0; file < 8; ++file) {
                binaryReadInt(stream, data);
                P_Color player = data == 1 ? P_WHITE : P_BLACK;
                binaryReadInt(stream, data);

                if (data != -1) {
                    auto type = (PieceType) data;
                    setPiece(pieces, {rank, file}, new Piece{player, type});
                } else
                    setPiece(pieces, {rank, file}, nullptr);
            }
        }
        stream.close();
        return true;
    }
    stream.close();
    return false;
}

void Chess::binaryWriteInt(ofstream &stream, int &data) const {
    stream.write((char *) &data, 4);
}

void Chess::binaryReadInt(ifstream &stream, int &data) const {
    stream.read((char *) &data, 4);
}

void Chess::deleteSave() {
    remove("prevState.sav");
}
