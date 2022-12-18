//
// Created by J-Bros on 12/17/2022.
//

#include <ctime>
#include "Chess.h"
#include "Input.h"

struct Chess::Piece {
    Color color;
    PieceType type;
    Location cell;
};

Frame Chess::frame{};

Chess::Chess() {
    setupBoard();
    frame.clear();
    setupBoardFrame();
    setupPiecesFrame();
    updateTimeFrame();
    updatePlayerFrame();
    updateCurrentCellFrame();
}

void Chess::startGame() {
    frame.updateDisplay();
    time_t startTime = time(nullptr);
    int key = 0;
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
                    if (selectedCell.equal(currentCell))
                        selectedCell.set(-1, -1);
                    else {
                        Piece *piece = getPiece(currentCell);
                        if (piece != nullptr && piece->color == currentPlayer) {
                            if (selectedCell.rank != -1)
                                resetCellFrame(selectedCell);
                            selectedCell.set(currentCell.rank, currentCell.file);
                        }
                    }
                    break;
                case Key::ESC: {
                    time_t delay = time(nullptr);
                    int option = 0;
                    do {
                        option = pauseMenu.selectOption();
                        switch (option) {
                            case 1:
                                switch (expertMenu.selectOption()) {
                                    case 0:
                                        expertMode = false;
                                        break;
                                    case 1:
                                        expertMode = true;
                                }
                                break;
                            case 2:
                                //... TODO: HELP
                                break;
                            case 3:
                                if (quitMenu.selectOption() == 1)
                                    return;
                        }
                    } while (option != 0);
                    startTime += time(nullptr) - delay;
                    break;
                }
                default:
                    continue;
            }
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

Chess::Piece *Chess::getPiece(const Location &cell) {
    Piece *current = nullptr;
    for (int i = 0; i < pieces.size(); ++i) {
        current = pieces.get(i);
        if (current->cell.equal(cell))
            break;
    }
    return current;
}

void Chess::setupBoard() {
    pieces.insert(new Piece{WHITE, ROOK, {0, 0}});
    pieces.insert(new Piece{WHITE, KNIGHT, {0, 1}});
    pieces.insert(new Piece{WHITE, BISHOP, {0, 2}});
    pieces.insert(new Piece{WHITE, QUEEN, {0, 3}});
    pieces.insert(new Piece{WHITE, KING, {0, 4}});
    pieces.insert(new Piece{WHITE, BISHOP, {0, 5}});
    pieces.insert(new Piece{WHITE, KNIGHT, {0, 6}});
    pieces.insert(new Piece{WHITE, ROOK, {0, 7}});

    pieces.insert(new Piece{BLACK, ROOK, {7, 0}});
    pieces.insert(new Piece{BLACK, KNIGHT, {7, 1}});
    pieces.insert(new Piece{BLACK, BISHOP, {7, 2}});
    pieces.insert(new Piece{BLACK, QUEEN, {7, 3}});
    pieces.insert(new Piece{BLACK, KING, {7, 4}});
    pieces.insert(new Piece{BLACK, BISHOP, {7, 5}});
    pieces.insert(new Piece{BLACK, KNIGHT, {7, 6}});
    pieces.insert(new Piece{BLACK, ROOK, {7, 7}});

    for (int i = 0; i < 8; ++i) {
        pieces.insert(new Piece{WHITE, PAWN, {1, i}});
        pieces.insert(new Piece{BLACK, PAWN, {6, i}});
    }
}

string Chess::timeToString(int seconds) {
    int hour = seconds / 3600;
    seconds %= 3600;
    int minute = seconds / 60;
    int second = seconds % 60;

    stringstream stream;
    stream << hour / 10 << hour % 10 << ':'
           << minute / 10 << minute % 10 << ':'
           << second / 10 << second % 10;
    return stream.str();
}

void Chess::setupBoardFrame() {
    for (int y = 4; y <= 16; y += 2) {
        frame.update(195, 4, y);
        for (int x = 5; x <= 35; ++x) {
            frame.update((x % 4) ? 196 : 197, x, y);
        }
        frame.update(180, 36, y);
    }

    frame.update(218, 4, 2);
    frame.update(192, 4, 18);
    for (int x = 5; x <= 35; ++x) {
        frame.update((x % 4) ? 196 : 194, x, 2);
        frame.update((x % 4) ? 196 : 193, x, 18);
    }
    frame.update(191, 36, 2);
    frame.update(217, 36, 18);

    for (int y = 3; y <= 17; y += 2) {
        for (int x = 1; x <= 9; ++x) {
            frame.update(179, x * 4, y);
        }
    }

    for (int x = 1; x < Frame::FRAME_X - 1; ++x) {
        frame.update(250, x, 1);
        frame.update(250, x, Frame::FRAME_Y - 2);
    }

    for (int y = 2; y < Frame::FRAME_Y - 2; ++y) {
        for (int x = 0; x < 2; ++x) {
            frame.update(250, 1 + x, y);
            frame.update(250, x + Frame::FRAME_X - 3, y);
        }
    }
}

void Chess::setupPiecesFrame() {
    for (int i = 0; i < this->pieces.size(); ++i) {
        Piece *piece = this->pieces.get(i);
        char ch = (piece->color == WHITE) ? piece->type : piece->type + 32;
        frame.update(ch, piece->cell.file * 4 + 6, piece->cell.rank * 2 + 3);
    }
}

void Chess::updatePlayerFrame() {
    string playerString = (currentPlayer == WHITE) ? "WHITE" : "BLACK";
    for (int i = 0; i < playerString.size(); ++i)
        frame.update(playerString[i], i + 4, 1);
}

void Chess::updateTimeFrame() {
    string timeString = timeToString(timePassed);
    for (int i = 0; i < timeString.size(); ++i)
        frame.update(timeString[i], i + Frame::FRAME_X - 12, 1);
}

void Chess::updateCurrentCellFrame() {
    for (int x = 0; x < 3; ++x) {
        frame.update(205, x + currentCell.file * 4 + 5, currentCell.rank * 2 + 2);
        frame.update(205, x + currentCell.file * 4 + 5, currentCell.rank * 2 + 4);
    }
    frame.update(186, currentCell.file * 4 + 4, currentCell.rank * 2 + 3);
    frame.update(186, currentCell.file * 4 + 8, currentCell.rank * 2 + 3);

    frame.update(201, currentCell.file * 4 + 4, currentCell.rank * 2 + 2);
    frame.update(187, currentCell.file * 4 + 8, currentCell.rank * 2 + 2);
    frame.update(200, currentCell.file * 4 + 4, currentCell.rank * 2 + 4);
    frame.update(188, currentCell.file * 4 + 8, currentCell.rank * 2 + 4);
}

void Chess::updateSelectedCellFrame() {
    for (int x = 0; x < 3; ++x) {
        frame.update(220, x + selectedCell.file * 4 + 5, selectedCell.rank * 2 + 2);
        frame.update(223, x + selectedCell.file * 4 + 5, selectedCell.rank * 2 + 4);
    }
    frame.update(222, selectedCell.file * 4 + 4, selectedCell.rank * 2 + 3);
    frame.update(221, selectedCell.file * 4 + 8, selectedCell.rank * 2 + 3);

    frame.update(201, selectedCell.file * 4 + 4, selectedCell.rank * 2 + 2);
    frame.update(187, selectedCell.file * 4 + 8, selectedCell.rank * 2 + 2);
    frame.update(200, selectedCell.file * 4 + 4, selectedCell.rank * 2 + 4);
    frame.update(188, selectedCell.file * 4 + 8, selectedCell.rank * 2 + 4);
}

void Chess::resetCellFrame(const Location &cell) {
    for (int x = 0; x < 3; ++x) {
        frame.update(196, x + cell.file * 4 + 5, cell.rank * 2 + 2);
        frame.update(196, x + cell.file * 4 + 5, cell.rank * 2 + 4);
    }
    frame.update(179, cell.file * 4 + 4, cell.rank * 2 + 3);
    frame.update(179, cell.file * 4 + 8, cell.rank * 2 + 3);

    frame.update((cell.rank == 0) ? (cell.file == 0) ? 218 : 194 : (cell.file == 0) ? 195 : 197,
                 cell.file * 4 + 4, cell.rank * 2 + 2);
    frame.update((cell.rank == 0) ? (cell.file == 7) ? 191 : 194 : (cell.file == 7) ? 180 : 197,
                 cell.file * 4 + 8, cell.rank * 2 + 2);
    frame.update((cell.rank == 7) ? (cell.file == 0) ? 192 : 193 : (cell.file == 0) ? 195 : 197,
                 cell.file * 4 + 4, cell.rank * 2 + 4);
    frame.update((cell.rank == 7) ? (cell.file == 7) ? 217 : 193 : (cell.file == 7) ? 180 : 197,
                 cell.file * 4 + 8, cell.rank * 2 + 4);
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