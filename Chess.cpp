//
// Created by J-Bros on 12/17/2022.
//

#include <cmath>
#include <ctime>
#include "Chess.h"

Frame Chess::frame{};

Chess::Chess() {
    setupBoard();
    setupBoardFrame();
    setupPiecesFrame();
    updateTimeFrame();
    updatePointsFrame();
    updatePlayerFrame();
    updateCurrentCellFrame();
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
        true,
        &frame
};

Menu Chess::pauseMenu{
        "PAUSE MENU",
        new string[]{
                "Continue",
                "Expert Mode",
                "Help",
                "Quit"
        },
        4,
        true,
        &frame
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
        false,
        &frame
};

Menu Chess::expertMenu{
        "EXPERT MODE",
        new string[]{
                "No",
                "Yes"
        },
        2,
        true,
        &frame
};

Popup Chess::help{"../Help.txt"};

Ending Chess::checkmate{
        new string[]{"WIN", "CHECKMATE"},
        2, &frame
};
Ending Chess::stalemate{
        new string[]{"DRAW", "STALEMATE"},
        2, &frame
};
Ending Chess::deadPositions{
        new string[]{"DRAW", "DEAD POSITIONS"},
        2, &frame
};

void Chess::setupBoard() {
    pieces[0][0] = new Piece{WHITE, ROOK};
    pieces[0][1] = new Piece{WHITE, KNIGHT};
    pieces[0][2] = new Piece{WHITE, BISHOP};
    pieces[0][3] = new Piece{WHITE, QUEEN};
    pieces[0][4] = new Piece{WHITE, KING};
    pieces[0][5] = new Piece{WHITE, BISHOP};
    pieces[0][6] = new Piece{WHITE, KNIGHT};
    pieces[0][7] = new Piece{WHITE, ROOK};

    pieces[7][0] = new Piece{BLACK, ROOK};
    pieces[7][1] = new Piece{BLACK, KNIGHT};
    pieces[7][2] = new Piece{BLACK, BISHOP};
    pieces[7][3] = new Piece{BLACK, QUEEN};
    pieces[7][4] = new Piece{BLACK, KING};
    pieces[7][5] = new Piece{BLACK, BISHOP};
    pieces[7][6] = new Piece{BLACK, KNIGHT};
    pieces[7][7] = new Piece{BLACK, ROOK};

    for (int i = 0; i < 8; ++i) {
        pieces[1][i] = new Piece{WHITE, PAWN};
        pieces[6][i] = new Piece{BLACK, PAWN};
    }
}

void Chess::startGame() {
    frame.updateDisplay();
    getAllLegalMoves(currentPlayer == WHITE ? BLACK : WHITE, opponentLegalMoves, playerLegalMoves, true);
    getAllLegalMoves(currentPlayer, playerLegalMoves, opponentLegalMoves, false);
    time_t startTime = time(nullptr);
    int key;
    do {
        key = Input::getIfPressedKey();
        if (key) {
            switch (key) {
                case Key::UP:
                    resetCellFrame(currentCell);
                    currentCell.rank = currentCell.rank == 0 ?
                                       7 : currentCell.rank - 1;
                    break;
                case Key::DOWN:
                    resetCellFrame(currentCell);
                    currentCell.rank = (currentCell.rank + 1) % 8;
                    break;
                case Key::LEFT:
                    resetCellFrame(currentCell);
                    currentCell.file = currentCell.file == 0 ?
                                       7 : currentCell.file - 1;
                    break;
                case Key::RIGHT:
                    resetCellFrame(currentCell);
                    currentCell.file = (currentCell.file + 1) % 8;
                    break;
                case Key::DEBUG:
                    updateDebug();
                case Key::SELECT: {
                    if (selectedCell != currentCell) {
                        Piece *piece = pieces[currentCell.rank][currentCell.file];
                        if (selectedLegalMoves.contains(&currentCell)) {
                            move();
                            clearSelected();
                            if (!changePlayer())
                                return;
                        } else if (piece != nullptr && piece->getColor() == currentPlayer) {
                            clearSelected();
                            selectedCell.set(currentCell.rank, currentCell.file);
                            possibleMoves(selectedCell, selectedLegalMoves, opponentLegalMoves, false);
                            if (isCheck(currentPlayer))
                                selectedLegalMoves.intersect(playerLegalMoves);
                        } else {
                            if (!selectedLegalMoves.isEmpty())
                                clearSelected();
                        }
                    } else
                        clearSelected();
                }
                    break;
                case Key::ESC:
                    if (!pause_menu(startTime))
                        return;
                    break;
                default:
                    continue;
            }
            if (!expertMode && !selectedLegalMoves.isEmpty())
                updatePossibleCellFrame(selectedLegalMoves);
            if (selectedCell.rank != -1)
                updateSelectedCellFrame();
            updateCurrentCellFrame();
            frame.updateDisplay();
        } else {
            time_t currentTime = time(nullptr) - startTime;
            if (timePassed != currentTime) {
                timePassed = currentTime;
                updateTimeFrame();
                frame.updateDisplay();
            }
        }
    } while (true);
}

void Chess::clearSelected() {
    if (selectedCell.rank != -1)
        resetCellFrame(selectedCell);
    selectedCell.set(-1, -1);
    clearPossibles(selectedLegalMoves);
}

void Chess::clearPossibles(LinkedList<Location *> &allMoves) {
    while (!allMoves.isEmpty())
        resetCellFrame(*allMoves.removeFirst());
}

void Chess::move() {
    move(selectedCell, currentCell);
}

void Chess::move(const Location &from, const Location &to) {
    Piece *fromPiece = pieces[from.rank][from.file];
    Piece *toPiece = pieces[to.rank][to.file];

    if (toPiece != nullptr && fromPiece->getColor() == toPiece->getColor()) {
        int direction = to.file == 7 ? 1 : -1;
        move(from, {from.rank, 4 + direction * 2});
        move(to, {to.rank, 4 + direction});
        return;
    }

    int castlingIndex = fromPiece->getColor() == WHITE ? 1 : 4;
    if (fromPiece->getType() == KING)
        castlingRule[castlingIndex] = true;
    else if (fromPiece->getType() == ROOK) {
        int direction = from.file == 7 ? 1 : -1;
        castlingRule[castlingIndex + direction] = true;
    }

    if (fromPiece->getType() == PAWN) {
        if ((fromPiece->getColor() == WHITE) ? to.rank == 7 : to.rank == 0) {
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

        if ((fromPiece->getColor() == WHITE) ?
            from.rank == 1 && to.rank == 3 : from.rank == 6 && to.rank == 4) {
            enPassantSession = session;
            enPassantTo.set(to.rank, to.file);
        }

        if (session == enPassantSession + 1) {
            int rank = fromPiece->getColor() == WHITE ? to.rank - 1 : to.rank + 1;
            if (enPassantTo.equals(rank, to.file)) {
                pieces[enPassantTo.rank][enPassantTo.file] = nullptr;
                updatePieceFrame(enPassantTo);
                enPassantTo.set(-1, -1);
                enPassantSession = -1;
                ((currentPlayer == WHITE) ? whitePoints : blackPoints) += 1;
            }
        }
    }

    if (toPiece != nullptr)
        switch (toPiece->getType()) {
            case QUEEN:
                ((currentPlayer == WHITE) ? whitePoints : blackPoints) += 9;
                break;
            case BISHOP:
            case KNIGHT:
                ((currentPlayer == WHITE) ? whitePoints : blackPoints) += 3;
                break;
            case ROOK:
                ((currentPlayer == WHITE) ? whitePoints : blackPoints) += 5;
                break;
            case PAWN:
                ((currentPlayer == WHITE) ? whitePoints : blackPoints) += 1;
                break;
        }

    pieces[from.rank][from.file] = nullptr;
    pieces[to.rank][to.file] = fromPiece;

    updatePieceFrame(from);
    updatePieceFrame(to);
    updatePointsFrame();
    delete toPiece;
}

bool Chess::changePlayer() {
    currentPlayer = currentPlayer == WHITE ? BLACK : WHITE;
    currentCell.set(3, 3);
    ++session;
    updatePlayerFrame();
    return updateStatus();
}

Location *Chess::getKingLocation(Color player) {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece *piece = pieces[y][x];
            if (piece != nullptr && piece->getColor() == player && piece->getType() == KING)
                return new Location{y, x};
        }
    }
    return nullptr;
}

bool Chess::updateStatus() {
    opponentLegalMoves.clear();
    getAllLegalMoves(currentPlayer == WHITE ? BLACK : WHITE, opponentLegalMoves, playerLegalMoves, true);
    playerLegalMoves.clear();
    getAllLegalMoves(currentPlayer, playerLegalMoves, opponentLegalMoves, false);

    Location *king = getKingLocation(currentPlayer);
    LinkedList<Location *> list{};
    possibleMoves(*king, list, opponentLegalMoves, false);

    //...TODO:WORKING
    if (isCheck(currentPlayer)) {
        for (int rank = 0; rank < 8; ++rank) {
            for (int file = 0; file < 8; ++file) {
                Piece *piece = pieces[rank][file];
                if (piece != nullptr && piece->getColor() != currentPlayer) {
                    LinkedList<Location *> l{};
                    possibleMoves({rank, file}, l, playerLegalMoves, true);
                    if (l.contains(getKingLocation(currentPlayer))) {
                        for (int r = 0; r < 8; ++r) {
                            for (int f = 0; f < 8; ++f) {
                                Piece *p = pieces[r][f];
                                if (p != nullptr && p->getColor() == currentPlayer) {
                                    LinkedList<Location *> inList{};
                                    possibleMoves({r, f}, inList, opponentLegalMoves, false);
                                    inList.intersect(l);
                                    inList.iteratorReset();
                                    while (!inList.isIteratorEnd())
                                        list.insert(inList.iteratorNext());
                                }
                            }
                        }
                    }
                }
            }
        }
        playerLegalMoves.intersect(list);
    }

    if (playerLegalMoves.isEmpty()) {
        if (isCheck(currentPlayer)) {
            checkmate.pop();
            return false;
        } else {
            stalemate.pop();
            return false;
        }
    }

    if (isOnlyKing(WHITE) && isOnlyKing(BLACK)) {
        deadPositions.pop();
        return false;
    }
    return true;
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
                help.pop();
                break;
            case 3:
                if (quitMenu.selectOption() == 1)
                    return false;
        }
    } while (option != 0 && option != -1);
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
            cleanPossiblesFrame(selectedLegalMoves);
            if (selectedCell.rank != -1)
                updateSelectedCellFrame();
            updateCurrentCellFrame();
    }
}
