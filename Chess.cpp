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
        nullptr,
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
    updateStatus();
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
                            if (pieceCheckCount <= 1 || piece->getType() == KING) {
                                clearSelected();
                                selectedCell.set(currentCell.rank, currentCell.file);
                                possibleMoves(selectedCell, selectedLegalMoves, opponentLegalMoves, false);
                                if (isCheck(currentPlayer) && piece->getType() != KING)
                                    selectedLegalMoves.intersect(playerLegalMoves);
                            }
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

bool Chess::changePlayer() {
    currentPlayer = currentPlayer == WHITE ? BLACK : WHITE;
    currentCell.set(3, 3);
    ++session;
    updatePlayerFrame();
    return updateStatus();
}

bool Chess::updateStatus() {
    opponentLegalMoves.clear();
    getAllLegalMoves(currentPlayer == WHITE ? BLACK : WHITE, opponentLegalMoves, playerLegalMoves, true);
    playerLegalMoves.clear();
    getAllLegalMoves(currentPlayer, playerLegalMoves, opponentLegalMoves, false);

    Location *king = getKingLocation(currentPlayer);
    LinkedList<Location *> checkLegalMoves{};

    pieceCheckCount = 0;

    //...TODO:WORKING
    if (isCheck(currentPlayer)) {
        for (int o_rank = 0; o_rank < 8; ++o_rank) {
            for (int o_file = 0; o_file < 8; ++o_file) {
                Piece *o_piece = pieces[o_rank][o_file];
                if (o_piece != nullptr && o_piece->getColor() != currentPlayer && o_piece->getType() != KING) {
                    LinkedList<Location *> o_list{};
                    possibleMoves({o_rank, o_file}, o_list, playerLegalMoves, true);
                    if (o_list.contains(getKingLocation(currentPlayer))) {
                        ++pieceCheckCount;
                        if (o_piece->getType() != PAWN && o_piece->getType() != KNIGHT) {
                            o_list.remove(king);
                            o_list.iteratorReset();
                            while (!o_list.isIteratorEnd()) {
                                Location *cell = o_list.iteratorNext();
                                int king_rank_delta = king->rank - o_rank;
                                int king_file_delta = king->file - o_file;
                                int cell_rank_delta = cell->rank - o_rank;
                                int cell_file_delta = cell->file - o_file;
                                int king_rank_direction = (king_rank_delta != 0) ?
                                                          king_rank_delta / abs(king_rank_delta) : 0;
                                int king_file_direction = (king_file_delta != 0) ?
                                                          king_file_delta / abs(king_file_delta) : 0;
                                int cell_rank_direction = (cell_rank_delta != 0) ?
                                                          cell_rank_delta / abs(cell_rank_delta) : 0;
                                int cell_file_direction = (cell_file_delta != 0) ?
                                                          cell_file_delta / abs(cell_file_delta) : 0;

                                if (king_rank_direction != cell_rank_direction ||
                                    king_file_direction != cell_file_direction)
                                    o_list.remove(cell);
                            }
                        } else {
                            o_list.clear();
                        }
                        o_list.insert(new Location{o_rank, o_file});
                        for (int p_rank = 0; p_rank < 8; ++p_rank) {
                            for (int p_file = 0; p_file < 8; ++p_file) {
                                Piece *p_piece = pieces[p_rank][p_file];
                                if (p_piece != nullptr && p_piece->getColor() == currentPlayer &&
                                    p_piece->getType() != KING) {
                                    LinkedList<Location *> p_list{};
                                    possibleMoves({p_rank, p_file}, p_list, opponentLegalMoves, false);
                                    p_list.intersect(o_list);

                                    p_list.iteratorReset();
                                    while (!p_list.isIteratorEnd()) {
                                        Location *cell = p_list.iteratorNext();
                                        if (!checkLegalMoves.contains(cell))
                                            checkLegalMoves.insert(cell);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (pieceCheckCount == 1)
            playerLegalMoves.intersect(checkLegalMoves);
    }

    LinkedList<Location *> kingLegalMoves{};
    possibleMoves(*king, kingLegalMoves, opponentLegalMoves, false);

    if (playerLegalMoves.isEmpty() && kingLegalMoves.isEmpty()) {
        if (isCheck(currentPlayer)) {
            checkmate.setText(new string[]{"CHECKMATE", currentPlayer == WHITE ? "BLACK WON" : "WHITE WON"});
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
