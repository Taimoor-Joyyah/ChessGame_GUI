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
    setPiece({0,0}, new Piece{WHITE, ROOK});
    setPiece({0,1}, new Piece{WHITE, KNIGHT});
    setPiece({0,2}, new Piece{WHITE, BISHOP});
    setPiece({0,3}, new Piece{WHITE, QUEEN});
    setPiece({0,4}, new Piece{WHITE, KING});
    setPiece({0,5}, new Piece{WHITE, BISHOP});
    setPiece({0,6}, new Piece{WHITE, KNIGHT});
    setPiece({0,7}, new Piece{WHITE, ROOK});

    setPiece({7,0}, new Piece{BLACK, ROOK});
    setPiece({7,1}, new Piece{BLACK, KNIGHT});
    setPiece({7,2}, new Piece{BLACK, BISHOP});
    setPiece({7,3}, new Piece{BLACK, QUEEN});
    setPiece({7,4}, new Piece{BLACK, KING});
    setPiece({7,5}, new Piece{BLACK, BISHOP});
    setPiece({7,6}, new Piece{BLACK, KNIGHT});
    setPiece({7,7}, new Piece{BLACK, ROOK});

    for (int i = 0; i < 8; ++i) {
        setPiece({1,i}, new Piece{WHITE, PAWN});
        setPiece({6,i}, new Piece{BLACK, PAWN});
    }
}

void Chess::startGame() {
    frame.updateDisplay();
    updateStatus();
    time_t startTime = time(nullptr);
    int key;
    while (true) {
        Piece *currentPiece = getPiece(currentCell);
        Piece *selectedPiece = (selectedCell.rank == -1) ? nullptr : getPiece(selectedCell);

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
                case Key::SELECT: {
                    if (currentCell != selectedCell) {
                        if (selectedPiece != nullptr && selectedPiece->getLegalMoves().contains(&currentCell)) {
                            clearLegalMovesFrame();
                            move();
                            clearSelected();
                            if (!changePlayer()) {
                                return;
                            }
                        } else if (currentPiece != nullptr && currentPiece->getColor() == currentPlayer) {
                            clearLegalMovesFrame();
                            clearSelected();
                            if (!currentPiece->getLegalMoves().isEmpty())
                                selectedCell.set(currentCell.rank, currentCell.file);
                        } else {
                            if (selectedPiece != nullptr && !selectedPiece->getLegalMoves().isEmpty()) {
                                clearLegalMovesFrame();
                                clearSelected();
                            }
                        }
                    } else {
                        clearLegalMovesFrame();
                        clearSelected();
                    }
                }
                    break;
                case Key::ESC:
                    if (!pause_menu(startTime))
                        return;
                    break;
                default:
                    continue;
            }
            if (!expertMode)
                updateLegalMovesFrame();
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
    }
}

bool Chess::changePlayer() {
    currentPlayer = getOpponent();
    isWhite = !isWhite;
    currentCell.set(3, 3);
    ++session;
    updatePlayerFrame();
    return updateStatus();
}

bool Chess::updateStatus() {
    bool hasLegalMove = false;
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            Piece *piece = getPiece({rank,file});
            if (piece != nullptr && piece->getColor() == currentPlayer) {
                LinkedList<Location *> &moves = piece->getLegalMoves();
                getMoves({rank, file}, moves, false);

                setPiece({rank,file}, nullptr);
                moves.iteratorReset();
                while (!moves.isIteratorEnd()) {
                    Location *move = moves.iteratorNext();
                    Piece *temp = getPiece(*move);
                    setPiece(*move, piece);

                    if (isCheckOn(getKingLocation(currentPlayer)))
                        moves.remove(move);

                    setPiece(*move, temp);
                }
                setPiece({rank,file}, piece);

                hasLegalMove = hasLegalMove || !moves.isEmpty();
            }
        }
    }

    bool isCheck = isCheckOn(getKingLocation(currentPlayer));
    int castlingKingIndex = isWhite ? 1 : 4;
    if (!castlingRule[castlingKingIndex]) {
        int king_rank = isWhite ? 0 : 7;
        if (!isCheck) {
            for (int i = -1; i <= 1; i += 2) {
                if (!castlingRule[castlingKingIndex + i]) {
                    bool empty = true;
                    int left = (i == -1 ? 1 : 5);
                    int right = (i == -1 ? 3 : 6);
                    for (int j = left; j <= right; ++j) {
                        if (getPiece({king_rank, j}) != nullptr)
                            empty = false;
                    }
                    if (empty) {
                        Piece *king = getPiece(*getKingLocation(currentPlayer));
                        if (!isCheckOn(new Location{king_rank, 4 + i}) &&
                            !isCheckOn(new Location{king_rank, 4 + i * 2})) {
                            king->getLegalMoves().insert(new Location{king_rank, 4 + i * 2});
                        }
                    }
                }
            }
        }
    }


    if (!hasLegalMove) {
        if (isCheck) {
            checkmate.setText(new string[]{"CHECKMATE", isWhite ? "BLACK WON" : "WHITE WON"});
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
    if (selectedCell.rank != -1) {
        resetCellFrame(selectedCell);
    }
    selectedCell.set(-1, -1);
}

Color Chess::getOpponent() {
    return isWhite ? BLACK : WHITE;
}

Piece *Chess::getPiece(const Location &cell) {
    return pieces[cell.rank][cell.file];
}

void Chess::setPiece(const Location &cell, Piece *piece) {
    pieces[cell.rank][cell.file] = piece;
}

void Chess::move() {
    move(selectedCell, currentCell);
}

void Chess::move(Location &from, Location &to) {
    Piece *fromPiece = getPiece(from);
    Piece *toPiece = getPiece(to);

    if (fromPiece->getType() == KING && abs(from.file - to.file) == 2) {
        Location rook{from.rank, (to.file == 6 ? 7 : 0)};
        Location moveTo{from.rank, (to.file == 6 ? 5 : 3)};
        move(rook, moveTo);
    }

    int castlingIndex = isWhite ? 1 : 4;
    if (fromPiece->getType() == KING)
        castlingRule[castlingIndex] = true;
    else if (fromPiece->getType() == ROOK) {
        int direction = from.file == 7 ? 1 : -1;
        castlingRule[castlingIndex + direction] = true;
    }

    if (fromPiece->getType() == PAWN) {
        if (isWhite ? to.rank == 7 : to.rank == 0) {
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

        if (isWhite ?
            from.rank == 1 && to.rank == 3 : from.rank == 6 && to.rank == 4) {
            enPassantSession = session;
            enPassantTo.set(to.rank, to.file);
        }

        if (session == enPassantSession + 1) {
            int rank = isWhite ? to.rank - 1 : to.rank + 1;
            if (enPassantTo.equals(rank, to.file)) {
                setPiece(enPassantTo, nullptr);
                updatePieceFrame(enPassantTo);
                enPassantTo.set(-1, -1);
                enPassantSession = -1;
                (isWhite ? whitePoints : blackPoints) += 1;
            }
        }
    }

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

    setPiece(from, nullptr);
    setPiece(to, fromPiece);

    updatePieceFrame(from);
    updatePieceFrame(to);
    updatePointsFrame();

    delete toPiece;
}

Location *Chess::getKingLocation(Color player) {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece *piece = getPiece({y,x});
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
            clearLegalMovesFrame();
            if (selectedCell.rank != -1)
                updateSelectedCellFrame();
            updateCurrentCellFrame();
    }
}
