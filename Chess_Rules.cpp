//
// Created by J-Bros on 12/20/2022.
//

#include "Chess.h"

void Chess::possibleMoves(const Location &cell, LinkedList<Location *> &possibles) {
    Piece *currentPiece = pieces[cell.rank][cell.file];
    switch (currentPiece->getType()) {
        case ROOK:
            addStraight(cell, possibles);
            break;
        case BISHOP:
            addDiagonal(cell, possibles);
            break;
        case QUEEN:
            addStraight(cell, possibles);
            addDiagonal(cell, possibles);
            break;
        case KING:
            for (int rank = cell.rank - 1; rank <= cell.rank + 1; ++rank)
                for (int file = cell.file - 1; file <= cell.file + 1; ++file)
                    addLocation(rank, file, currentPiece, possibles, true);
            break;
        case KNIGHT:
            for (int rank = cell.rank - 2; rank <= cell.rank + 2; ++rank)
                for (int file = cell.file - 2; file <= cell.file + 2; ++file)
                    if (rank != cell.rank && file != cell.file && abs(cell.rank - rank) != abs(cell.file - file))
                        addLocation(rank, file, currentPiece, possibles, true);
            break;
        case PAWN: {
            int type = currentPiece->getColor() == WHITE ? 1 : -1;
            Piece *piece;
            int rank;
            int file;
            for (int i = 0; i < 2; ++i) {
                rank = cell.rank + type * (i + 1);
                file = cell.file;
                if (i == 0 || cell.rank == (7 + type) % 7) {
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
                    if ((piece != nullptr && piece->getColor() != currentPiece->getColor()) ||
                        (session == enPassantSession + 1 && enPassantTo.equal({cell.rank, file}))) {
                        possibles.insert(new Location{rank, file});
                    }
                }
            }
        }
    }
}

void Chess::addStraight(const Location &cell, LinkedList<Location *> &possibles) {
    Piece *currentPiece = pieces[cell.rank][cell.file];
    for (int rank = cell.rank + 1; rank < 8 &&
                                   addLocation(rank, cell.file, currentPiece, possibles); ++rank);
    for (int rank = cell.rank - 1; rank >= 0 &&
                                   addLocation(rank, cell.file, currentPiece, possibles); --rank);
    for (int file = cell.file + 1; file < 8 &&
                                   addLocation(cell.rank, file, currentPiece, possibles); ++file);
    for (int file = cell.file - 1; file >= 0 &&
                                   addLocation(cell.rank, file, currentPiece, possibles); --file);
}

void Chess::addDiagonal(const Location &cell, LinkedList<Location *> &possibles) {
    Piece *currentPiece = pieces[cell.rank][cell.file];
    for (int rank = cell.rank + 1, file = cell.file + 1;
         rank < 8 && file < 8 && addLocation(rank, file, currentPiece, possibles); ++rank, ++file);
    for (int rank = cell.rank + 1, file = cell.file - 1;
         rank < 8 && file >= 0 && addLocation(rank, file, currentPiece, possibles); ++rank, --file);
    for (int rank = cell.rank - 1, file = cell.file + 1;
         rank >= 0 && file < 8 && addLocation(rank, file, currentPiece, possibles); --rank, ++file);
    for (int rank = cell.rank - 1, file = cell.file - 1;
         rank >= 0 && file >= 0 && addLocation(rank, file, currentPiece, possibles); --rank, --file);
}

bool
Chess::addLocation(int rank, int file, const Piece *currentPiece, LinkedList<Location *> &possibles, bool couldLeap) {
    if (file >= 0 && file <= 7 && rank >= 0 && rank <= 7) {
        Piece *piece = pieces[rank][file];
        if (piece != nullptr) {
            if (piece->getColor() != currentPiece->getColor())
                possibles.insert(new Location{rank, file});
            return couldLeap;
        }
        possibles.insert(new Location{rank, file});
    }
    return true;
}