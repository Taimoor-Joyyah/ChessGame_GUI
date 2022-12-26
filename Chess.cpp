//
// Created by J-Bros on 12/17/2022.
//

#include <cmath>
#include <ctime>
#include <fstream>
#include "Chess.h"

Frame Chess::frame{};

Chess::Chess(bool isContinue) {
    if (isContinue) {
        loaded = load();
    } else {
        deleteSave();
        setupBoard();
    }
    setupBoardFrame();
    setupPiecesFrame();
    updateTimeFrame();
    updatePointsFrame();
    updatePlayerFrame();
    updateCurrentCellFrame();
    updateSelectedCellFrame();
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
                "Save",
                "Help",
                "Quit"
        },
        5,
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
    setPiece({0, 0}, new Piece{BLACK, ROOK});
    setPiece({0, 1}, new Piece{BLACK, KNIGHT});
    setPiece({0, 2}, new Piece{BLACK, BISHOP});
    setPiece({0, 3}, new Piece{BLACK, QUEEN});
    setPiece({0, 4}, new Piece{BLACK, KING});
    setPiece({0, 5}, new Piece{BLACK, BISHOP});
    setPiece({0, 6}, new Piece{BLACK, KNIGHT});
    setPiece({0, 7}, new Piece{BLACK, ROOK});

    setPiece({7, 0}, new Piece{WHITE, ROOK});
    setPiece({7, 1}, new Piece{WHITE, KNIGHT});
    setPiece({7, 2}, new Piece{WHITE, BISHOP});
    setPiece({7, 3}, new Piece{WHITE, QUEEN});
    setPiece({7, 4}, new Piece{WHITE, KING});
    setPiece({7, 5}, new Piece{WHITE, BISHOP});
    setPiece({7, 6}, new Piece{WHITE, KNIGHT});
    setPiece({7, 7}, new Piece{WHITE, ROOK});

    for (int i = 0; i < 8; ++i) {
        setPiece({1, i}, new Piece{BLACK, PAWN});
        setPiece({6, i}, new Piece{WHITE, PAWN});
    }
}

void Chess::startGame() {
    if (!loaded)
        return;
    updateStatus();
    updateLegalMovesFrame();
    frame.updateDisplay();
    time_t startTime = time(nullptr) - timePassed;
    int key;
    while (true) {
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
                    if (currentCell == selectedCell)
                        clearSelected();
                    else if (!selectCell())
                        return;
                }
                    break;
                case Key::ESC:
                    if (!pause_menu(startTime))
                        return;
                    break;
                default:
                    continue;
            }
            updateFrame();
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

void Chess::updateFrame() {
    if (!expertMode)
        updateLegalMovesFrame();
    updateSelectedCellFrame();
    updateCurrentCellFrame();
    frame.updateDisplay();
}

bool Chess::selectCell() {
    Piece *currentPiece = getPiece(currentCell);
    Piece *selectedPiece = (selectedCell.rank == -1) ? nullptr : getPiece(selectedCell);

    if (selectedPiece != nullptr && selectedPiece->getLegalMoves().contains(&currentCell)) {
        Location temp{selectedCell.rank, selectedCell.file};
        clearSelected();
        move(temp);
        if (!changePlayer()) {
            deleteSave();
            return false;
        }
    } else if (currentPiece != nullptr && currentPiece->getColor() == currentPlayer) {
        clearSelected();
        if (!currentPiece->getLegalMoves().isEmpty())
            selectedCell.set(currentCell.rank, currentCell.file);
    } else if (selectedPiece != nullptr && !selectedPiece->getLegalMoves().isEmpty())
        clearSelected();
    return true;
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
    bool hasLegalMove = simulate();
    bool isCheck = isCheckOn(getKingLocation(currentPlayer));
    if (!isCheck)
        addIfCastling();
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

void Chess::addIfCastling() {
    int castlingKingIndex = isWhite ? 4 : 1;
    if (!castlingRule[castlingKingIndex]) {
        int king_rank = isWhite ? 7 : 0;
        for (int i = -1; i <= 1; i += 2) {
            if (!castlingRule[castlingKingIndex + i] &&
                ((i == 1) ? getPiece({king_rank, 6}) == nullptr :
                 getPiece({king_rank, 1}) == getPiece({king_rank, 2}))) {
                Piece *king = getPiece(*getKingLocation(currentPlayer));
                if (king->getLegalMoves().contains(new Location{king_rank, 4 + i}) &&
                    !isCheckOn(new Location{king_rank, 4 + i * 2})) {
                    king->getLegalMoves().insert(new Location{king_rank, 4 + i * 2});
                }
            }
        }
    }
}

bool Chess::simulate() {
    bool hasLegalMove = false;
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            Piece *piece = getPiece({rank, file});
            if (piece != nullptr && piece->getColor() == currentPlayer) {
                LinkedList<Location *> &moves = piece->getLegalMoves();
                getMoves({rank, file}, moves, false);
                setPiece({rank, file}, nullptr);
                moves.iteratorReset();
                while (!moves.isIteratorEnd()) {
                    Location *move = moves.iteratorNext();
                    Piece *temp = getPiece(*move);
                    setPiece(*move, piece);
                    if (isCheckOn(getKingLocation(currentPlayer)))
                        moves.remove(move);
                    setPiece(*move, temp);
                }
                setPiece({rank, file}, piece);
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
    Piece *fromPiece = getPiece(from);
    Piece *toPiece = getPiece(to);

    castling(from, to);
    evaluateCastling(from);

    if (fromPiece->getType() == PAWN) {
        promotion(to, fromPiece);
        evaluateEnPassant(from, to);
        enPassantMove(to);
    }

    evaluatePoints(toPiece);

    setPiece(from, nullptr);
    setPiece(to, fromPiece);

    updatePieceFrame(from);
    updatePieceFrame(to);
    updatePointsFrame();

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
            setPiece(enPassantTo, nullptr);
            updatePieceFrame(enPassantTo);
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
    if (getPiece(from)->getType() == KING && abs(from.file - to.file) == 2) {
        Location rook{from.rank, (to.file == 6 ? 7 : 0)};
        Location moveTo{from.rank, (to.file == 6 ? 5 : 3)};
        move(rook, moveTo);
    }
}

void Chess::clearSelected() {
    clearLegalMovesFrame();
    resetCellFrame(selectedCell);
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

void Chess::evaluateCastling(const Location &cell) {
    Piece *piece = getPiece(cell);
    int castlingIndex = isWhite ? 4 : 1;
    if (piece->getType() == KING)
        castlingRule[castlingIndex] = true;
    else if (piece->getType() == ROOK) {
        int direction = cell.file == 7 ? 1 : -1;
        castlingRule[castlingIndex + direction] = true;
    }
}

Location *Chess::getKingLocation(Color player) {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece *piece = getPiece({y, x});
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
            clearLegalMovesFrame();
            if (selectedCell.rank != -1)
                updateSelectedCellFrame();
            updateCurrentCellFrame();
    }
}

void Chess::save() {
    ofstream stream("prevState.sav", ios::binary | ios::out);
    if (stream.is_open()) {
        int temp = (expertMode ? 1 : 0);
        binaryWriteInt(stream, temp);
        temp = (isWhite ? 1 : 0);
        binaryWriteInt(stream, temp);
        binaryWriteInt(stream, (int &) timePassed);
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
            temp = (i ? 1 : 0);
            binaryWriteInt(stream, temp);
        }
        for (int rank = 0; rank < 8; ++rank) {
            for (int file = 0; file < 8; ++file) {
                Piece *piece = getPiece({rank, file});
                if (piece != nullptr) {
                    temp = (piece->getColor() == WHITE ? 1 : 0);
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
        expertMode = (data == 1);
        binaryReadInt(stream, data);
        currentPlayer = (data == 1) ? WHITE : BLACK;
        isWhite = (currentPlayer == WHITE);
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
            i = (data == 1);
        }
        for (int rank = 0; rank < 8; ++rank) {
            for (int file = 0; file < 8; ++file) {
                binaryReadInt(stream, data);
                Color player = (data == 1) ? WHITE : BLACK;
                binaryReadInt(stream, data);

                if (data != -1) {
                    auto type = (PieceType) data;
                    setPiece({rank, file}, new Piece{player, type});
                } else
                    setPiece({rank, file}, nullptr);
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
