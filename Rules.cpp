//
// Created by J-Bros on 12/18/2022.
//

#include "Rules.h"

void Rules::possibleMoves(const Location &cell, Piece *pieces[8][8], LinkedList<Location *> &possibles) {
    Piece *currentPiece = pieces[cell.rank][cell.file];

    switch (currentPiece->getType()) {
        case KING:
            for (int rank = cell.rank - 1; rank <= cell.rank + 1; ++rank) {
                for (int file = cell.file - 1; file <= cell.file + 1; ++file) {
                    if (rank != 0 || file != 0) {
                        if (file >= 0 && file <= 7 && rank >= 0 && rank <= 7) {
                            Piece *piece = pieces[rank][file];
                            if (piece == nullptr || piece->getColor() != currentPiece->getColor()) {
                                possibles.insert(new Location{rank, file});
                            }
                        }
                    }
                }
            }
            break;
        case QUEEN:
            break;
        case BISHOP:
            break;
        case KNIGHT:
            break;
        case ROOK:

            break;
        case PAWN:
            break;
    }
}