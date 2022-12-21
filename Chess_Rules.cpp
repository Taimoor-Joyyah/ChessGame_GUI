//
// Created by J-Bros on 12/20/2022.
//

#include "Chess.h"

void Chess::possibleMoves(const Location &cell, LinkedList<Location *> &possibles, bool isChecking) {
    Piece *currentPiece = pieces[cell.rank][cell.file];
    switch (currentPiece->getType()) {
        case ROOK:
            addStraight(cell, possibles, isChecking);
            break;
        case BISHOP:
            addDiagonal(cell, possibles, isChecking);
            break;
        case QUEEN:
            addStraight(cell, possibles, isChecking);
            addDiagonal(cell, possibles, isChecking);
            break;
        case KING: {
            for (int rank = cell.rank - 1; rank <= cell.rank + 1; ++rank)
                for (int file = cell.file - 1; file <= cell.file + 1; ++file) {
                    if (!checkPossibles.contains(new Location{rank, file}))
                        addLocation(rank, file, currentPiece, possibles, false, isChecking);
                }
            int castlingKingIndex = (currentPiece->getColor() == WHITE) ? 1 : 4;
            if (!castlingRule[castlingKingIndex]) {
                if (!checkPossibles.contains(new Location{cell.rank, cell.file})) {
                    for (int i = 0; i < 2; ++i) {
                        if (!castlingRule[castlingKingIndex + (i == 0 ? -1 : 1)]) {
                            bool empty = true;
                            for (int j = (i == 0 ? 1 : 5); j <= (i == 0 ? 3 : 6); ++j) {
                                if (pieces[cell.rank][j] != nullptr)
                                    empty = false;
                            }
                            if (empty)
                                if (!checkPossibles.contains(new Location{cell.rank, cell.file - 1 + (2 * i)}) &&
                                    !checkPossibles.contains(new Location{cell.rank, cell.file - 2 + (4 * i)})) {
                                    possibles.insert(new Location{cell.rank, i * 7});
                                }
                        }
                    }
                }
            }
        }
            break;
        case KNIGHT:
            for (int rank = cell.rank - 2; rank <= cell.rank + 2; ++rank)
                for (int file = cell.file - 2; file <= cell.file + 2; ++file)
                    if (rank != cell.rank && file != cell.file && abs(cell.rank - rank) != abs(cell.file - file))
                        addLocation(rank, file, currentPiece, possibles, false, true);
            break;
        case PAWN: {
            int type = currentPiece->getColor() == WHITE ? 1 : -1;
            Piece *piece;
            int rank;
            int file;
            for (int i = 0; i < 2; ++i) {
                rank = cell.rank + type * (i + 1);
                file = cell.file;
                if (!isChecking && (i == 0 || cell.rank == (7 + type) % 7)) {
                    piece = pieces[rank][file];
                    if (piece == nullptr)
                        possibles.insert(new Location{rank, file});
                    else
                        break;
                }
            }
            for (int i = 0; i < 2; ++i) {
                rank = cell.rank + type;
                file = cell.file - 1 + i * 2;
                if (file >= 0 && file <= 7) {
                    piece = pieces[rank][file];
                    if (isChecking || ((piece != nullptr && piece->getColor() != currentPiece->getColor()) ||
                                       (session == enPassantSession + 1 && enPassantTo.equals(cell.rank, file)))) {
                        possibles.insert(new Location{rank, file});
                    }
                }
            }
        }
    }
}

void Chess::addStraight(const Location &cell, LinkedList<Location *> &possibles, bool isChecking) {
    Piece *currentPiece = pieces[cell.rank][cell.file];
    for (int rank = cell.rank + 1; rank < 8 &&
                                   addLocation(rank, cell.file, currentPiece, possibles, isChecking); ++rank);
    for (int rank = cell.rank - 1; rank >= 0 &&
                                   addLocation(rank, cell.file, currentPiece, possibles, isChecking); --rank);
    for (int file = cell.file + 1; file < 8 &&
                                   addLocation(cell.rank, file, currentPiece, possibles, isChecking); ++file);
    for (int file = cell.file - 1; file >= 0 &&
                                   addLocation(cell.rank, file, currentPiece, possibles, isChecking); --file);
}

void Chess::addDiagonal(const Location &cell, LinkedList<Location *> &possibles, bool isChecking) {
    Piece *currentPiece = pieces[cell.rank][cell.file];
    for (int rank = cell.rank + 1, file = cell.file + 1;
         rank < 8 && file < 8 && addLocation(rank, file, currentPiece, possibles, isChecking); ++rank, ++file);
    for (int rank = cell.rank + 1, file = cell.file - 1;
         rank < 8 && file >= 0 && addLocation(rank, file, currentPiece, possibles, isChecking); ++rank, --file);
    for (int rank = cell.rank - 1, file = cell.file + 1;
         rank >= 0 && file < 8 && addLocation(rank, file, currentPiece, possibles, isChecking); --rank, ++file);
    for (int rank = cell.rank - 1, file = cell.file - 1;
         rank >= 0 && file >= 0 && addLocation(rank, file, currentPiece, possibles, isChecking); --rank, --file);
}

bool
Chess::addLocation(int rank, int file, const Piece *currentPiece, LinkedList<Location *> &possibles, bool isChecking,
                   bool couldLeap) {
    if (file >= 0 && file <= 7 && rank >= 0 && rank <= 7) {
        Piece *piece = pieces[rank][file];
        if (piece != nullptr) {
            if (isChecking || piece->getColor() != currentPiece->getColor())
                possibles.insert(new Location{rank, file});
            return couldLeap;
        }
        possibles.insert(new Location{rank, file});
    }
    return true;
}

void Chess::setCheckPossibles() {
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            Piece *piece = pieces[rank][file];
            if (piece != nullptr && piece->getColor() != currentPlayer) {
                LinkedList<Location *> list{};
                possibleMoves({rank, file}, list, true);
                checkPossibles.merge(list);
            }
        }
    }
}

bool Chess::isCheck(Color player) {
    return checkPossibles.contains(getKingLocation(player));
}

bool Chess::isOnlyKing(Color player) {
    for (auto &rank: pieces)
        for (auto piece: rank)
            if (piece != nullptr &&
                piece->getColor() == player &&
                piece->getType() != KING)
                return false;
    return true;
}