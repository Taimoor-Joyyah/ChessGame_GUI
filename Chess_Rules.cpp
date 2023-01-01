//
// Created by J-Bros on 12/20/2022.
//

#include "Chess.h"

void Chess::getMoves(Piece *pieceList[8][8], const Location &cell, LinkedList<Location *> &moves, bool isSimulating) {
    moves.clear();
    Piece *currentPiece = getPiece(cell);
    switch (currentPiece->getType()) {
        case ROOK:
            addStraight(pieceList, cell, moves, isSimulating);
            break;
        case BISHOP:
            addDiagonal(pieceList, cell, moves, isSimulating);
            break;
        case QUEEN:
            addStraight(pieceList, cell, moves, isSimulating);
            addDiagonal(pieceList, cell, moves, isSimulating);
            break;
        case KING:
            for (int rank = cell.rank - 1; rank <= cell.rank + 1; ++rank)
                for (int file = cell.file - 1; file <= cell.file + 1; ++file)
                    if (rank != 0 || file != 0)
                        addLocation(pieceList, rank, file, currentPiece, moves, isSimulating, isSimulating);
            break;
        case KNIGHT:
            for (int rank = cell.rank - 2; rank <= cell.rank + 2; ++rank)
                for (int file = cell.file - 2; file <= cell.file + 2; ++file)
                    if (rank != cell.rank && file != cell.file && abs(cell.rank - rank) != abs(cell.file - file))
                        addLocation(pieceList, rank, file, currentPiece, moves, isSimulating, true);
            break;
        case PAWN: {
            int type = currentPiece->getColor() == P_WHITE ? -1 : 1;
            Piece *piece;
            int rank;
            int file;
            for (int i = 0; i < 2; ++i) {
                rank = cell.rank + type * (i + 1);
                file = cell.file;
                if (!isSimulating && (i == 0 || cell.rank == (7 + type) % 7)) {
                    piece = getPiece({rank, file});
                    if (piece == nullptr)
                        moves.insert(new Location{rank, file});
                    else
                        break;
                }
            }
            for (int i = 0; i < 2; ++i) {
                rank = cell.rank + type;
                file = cell.file - 1 + i * 2;
                if (file >= 0 && file <= 7) {
                    piece = getPiece({rank, file});
                    if (isSimulating || ((piece != nullptr && piece->getColor() != currentPiece->getColor()) ||
                                         (session == enPassantSession + 1 && enPassantTo.equals(cell.rank, file)))) {
                        moves.insert(new Location{rank, file});
                    }
                }
            }
        }
    }
}

void
Chess::addStraight(Piece *pieceList[8][8], const Location &cell, LinkedList<Location *> &moves, bool isSimulating) {
    Piece *currentPiece = getPiece(cell);
    for (int rank = cell.rank + 1; rank < 8 &&
            addLocation(pieceList, rank, cell.file, currentPiece, moves, isSimulating, false); ++rank);
    for (int rank = cell.rank - 1; rank >= 0 &&
            addLocation(pieceList, rank, cell.file, currentPiece, moves, isSimulating, false); --rank);
    for (int file = cell.file + 1; file < 8 &&
            addLocation(pieceList, cell.rank, file, currentPiece, moves, isSimulating, false); ++file);
    for (int file = cell.file - 1; file >= 0 &&
            addLocation(pieceList, cell.rank, file, currentPiece, moves, isSimulating, false); --file);
}

void
Chess::addDiagonal(Piece *pieceList[8][8], const Location &cell, LinkedList<Location *> &moves, bool isSimulating) {
    Piece *currentPiece = getPiece(cell);
    for (int rank = cell.rank + 1, file = cell.file + 1;
         rank < 8 && file < 8 && addLocation(pieceList, rank, file, currentPiece, moves, isSimulating, false); ++rank, ++file);
    for (int rank = cell.rank + 1, file = cell.file - 1;
         rank < 8 && file >= 0 && addLocation(pieceList, rank, file, currentPiece, moves, isSimulating, false); ++rank, --file);
    for (int rank = cell.rank - 1, file = cell.file + 1;
         rank >= 0 && file < 8 && addLocation(pieceList, rank, file, currentPiece, moves, isSimulating, false); --rank, ++file);
    for (int rank = cell.rank - 1, file = cell.file - 1;
         rank >= 0 && file >= 0 && addLocation(pieceList, rank, file, currentPiece, moves, isSimulating, false); --rank, --file);
}

bool
Chess::addLocation(Piece *pieceList[8][8], int rank, int file, const Piece *currentPiece, LinkedList<Location *> &moves,
                   bool isSimulating, bool couldLeap) {
    if (file >= 0 && file <= 7 && rank >= 0 && rank <= 7) {
        Piece *piece = pieceList[rank][file];
        if (piece != nullptr) {
            if (isSimulating || piece->getColor() != currentPiece->getColor())
                moves.insert(new Location{rank, file});
            return couldLeap;
        }
        moves.insert(new Location{rank, file});
    }
    return true;
}

bool Chess::isCheckOn(Piece *pieceList[8][8], Location *cell) {
    for (int rank = 0; rank < 8; ++rank)
        for (int file = 0; file < 8; ++file) {
            Piece *piece = pieceList[rank][file];
            if (piece != nullptr && piece->getColor() == (isWhite ? P_BLACK : P_WHITE)) {
                LinkedList<Location *> list{};
                getMoves(pieceList, {rank, file}, list, true);
                if (list.contains(cell))
                    return true;
            }
        }
    return false;
}