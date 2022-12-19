//
// Created by J-Bros on 12/17/2022.
//

#include <ctime>
#include "Chess.h"
#include "Input.h"

Frame Chess::frame{};

Chess::Chess() {
    setupBoard();
    setupBoardFrame();
    setupPiecesFrame();
    updateTimeFrame();
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
        &frame
};

Menu Chess::expertMenu{
        "EXPERT MODE",
        new string[]{
                "No",
                "Yes"
        },
        2,
        &frame
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
                        while (!possibles.isEmpty())
                            resetCellFrame(*possibles.removeFirst());

                    } else {
                        Piece *piece = pieces[currentCell.rank][currentCell.file];
                        if (piece != nullptr && piece->getColor() == currentPlayer) {
                            if (selectedCell.rank != -1) {
                                resetCellFrame(selectedCell);
                                while (!possibles.isEmpty())
                                    resetCellFrame(*possibles.removeFirst());
                            }
                            selectedCell.set(currentCell.rank, currentCell.file);
                            Rules::possibleMoves(selectedCell, pieces, possibles);
                        } else {
                            possibles.iteratorReset();
                            while (!possibles.isIteratorEnd()) {
                                Location *location = possibles.iteratorNext();
                                if (currentCell.equal(*location)) {
                                    move();
                                    resetCellFrame(selectedCell);
                                    selectedCell.set(-1, -1);
                                    while (!possibles.isEmpty())
                                        resetCellFrame(*possibles.removeFirst());
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
            if (!expertMode && !possibles.isEmpty())
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
    updatePieceFrame(selectedCell);
    updatePieceFrame(currentCell);
}

void Chess::move(const Location &from, const Location &to) {
    Piece *fromPiece = pieces[from.rank][from.file];
    Piece *toPiece = pieces[to.rank][to.file];

    pieces[from.rank][from.file] = nullptr;
    pieces[to.rank][to.file] = fromPiece;

    delete toPiece;
}

void Chess::changePlayer() {
    currentPlayer = currentPlayer == WHITE ? BLACK : WHITE;
    currentCell.set(3, 3);
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
                //... TODO: HELP
                break;
            case 3:
                if (quitMenu.selectOption() == 1)
                    return false;
        }
    } while (option != 0);
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
            possibles.iteratorReset();
            while (!possibles.isIteratorEnd())
                resetCellFrame(*possibles.iteratorNext());
            if (selectedCell.rank != -1)
                updateSelectedCellFrame();
            updateCurrentCellFrame();
    }
}