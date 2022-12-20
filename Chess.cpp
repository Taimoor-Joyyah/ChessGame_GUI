//
// Created by J-Bros on 12/17/2022.
//

#include <cmath>
#include <ctime>
#include "Chess.h"
#include "Input.h"

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
                case Key::SELECT:
                    if (selectedCell.equal(currentCell)) {
                        selectedCell.set(-1, -1);
                        while (!selectedPossibles.isEmpty())
                            resetCellFrame(*selectedPossibles.removeFirst());

                    } else {
                        Piece *piece = pieces[currentCell.rank][currentCell.file];
                        if (piece != nullptr && piece->getColor() == currentPlayer) {
                            if (selectedCell.rank != -1) {
                                resetCellFrame(selectedCell);
                                while (!selectedPossibles.isEmpty())
                                    resetCellFrame(*selectedPossibles.removeFirst());
                            }
                            selectedCell.set(currentCell.rank, currentCell.file);
                            possibleMoves(selectedCell, selectedPossibles);
                        } else {
                            selectedPossibles.iteratorReset();
                            while (!selectedPossibles.isIteratorEnd()) {
                                Location *location = selectedPossibles.iteratorNext();
                                if (currentCell.equal(*location)) {
                                    move();
                                    resetCellFrame(selectedCell);
                                    selectedCell.set(-1, -1);
                                    while (!selectedPossibles.isEmpty())
                                        resetCellFrame(*selectedPossibles.removeFirst());
                                    changePlayer();
                                    break;
                                }
                            }
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
            if (!expertMode && !selectedPossibles.isEmpty())
                updatePossibleCellFrame();
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

void Chess::move() {
    move(selectedCell, currentCell);
}

void Chess::move(const Location &from, const Location &to) {
    Piece *fromPiece = pieces[from.rank][from.file];
    Piece *toPiece = pieces[to.rank][to.file];

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
            if (enPassantTo.equal({rank , to.file})) {
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

void Chess::changePlayer() {
    currentPlayer = currentPlayer == WHITE ? BLACK : WHITE;
    currentCell.set(3, 3);
    ++session;
    updatePlayerFrame();
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
            selectedPossibles.iteratorReset();
            while (!selectedPossibles.isIteratorEnd())
                resetCellFrame(*selectedPossibles.iteratorNext());
            if (selectedCell.rank != -1)
                updateSelectedCellFrame();
            updateCurrentCellFrame();
    }
}