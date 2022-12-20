//
// Created by J-Bros on 12/18/2022.
//

#include "Chess.h"

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

    for (int i = 0; i < 8; ++i) {
        frame.update(56 - i, 2, i * 2 + 3);
        frame.update(65 + i, i * 4 + 6, 19);
    }
}

void Chess::setupPiecesFrame() {
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            updatePieceFrame({rank, file});
        }
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

void Chess::updatePointsFrame() {
    stringstream  stream;
    stream << "W>";
    stream << whitePoints / 10;
    stream << whitePoints % 10;
    stream << '-';
    stream << blackPoints / 10;
    stream << blackPoints % 10;
    stream << "<B";
    string points = stream.str();
    for (int i = 0; i < 9; ++i)
        frame.update(points[i], i + Frame::FRAME_X - 25, 1);
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

void Chess::updatePossibleCellFrame() {
    for (int i = 0; i < selectedPossibles.size(); ++i) {
        Location *cell = selectedPossibles.get(i);
        int ch;
        for (int x = 0; x < 3; ++x) {
            ch = frame.getChar(x + cell->file * 4 + 5, cell->rank * 2 + 2);
            frame.update(ch == 223 - 256 ? 219 : 220, x + cell->file * 4 + 5, cell->rank * 2 + 2);
            ch = frame.getChar(x + cell->file * 4 + 5, cell->rank * 2 + 4);
            frame.update(ch == 220 - 256 ? 219 : 223, x + cell->file * 4 + 5, cell->rank * 2 + 4);
        }
        ch = frame.getChar(cell->file * 4 + 4, cell->rank * 2 + 3);
        frame.update(ch == 221 - 256 ? 219 : 222, cell->file * 4 + 4, cell->rank * 2 + 3);
        ch = frame.getChar(cell->file * 4 + 8, cell->rank * 2 + 3);
        frame.update(ch == 222 - 256 ? 219 : 221, cell->file * 4 + 8, cell->rank * 2 + 3);
    }
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

void Chess::updatePieceFrame(const Location &cell) {
    Piece *piece = pieces[cell.rank][cell.file];
    if (piece != nullptr) {
        char ch = (piece->getColor() == WHITE) ? piece->getType() : piece->getType() + 32;
        frame.update(ch, cell.file * 4 + 6, cell.rank * 2 + 3);
    } else
        frame.update(32, cell.file * 4 + 6, cell.rank * 2 + 3);
}
