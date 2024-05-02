//
// Created by mtaimoor on 9/8/2023.
//

#include "Engine.h"
#include "Chess.h"
#include <thread>
#include <algorithm>

Move Engine::evaluateBestMove(Piece *pieces[8][8], P_Color player, int power) {
    srand(time(nullptr));
    GameState gameState{pieces, player};

    std::vector<Move> allPossibleMoves = getAllPossibleMoves(gameState);
    int totalMoves = allPossibleMoves.size();

    if (power == 0)
        return allPossibleMoves[rand() % totalMoves];

    int scores[totalMoves];

    std::thread threads[totalMoves];
    for (int i = 0; i < totalMoves; i++) {
        threads[i] = std::thread([&, i] {
            Piece *piece = Chess::getPiece(gameState.pieces, allPossibleMoves[i].fromLocation);
            PieceType type = piece->getType();
            scores[i] = minimax(movePiece(&gameState, allPossibleMoves[i]), power - 1, INT_MIN, INT_MAX, false);
            piece->setType(type);
        });
    }

    for (auto &th: threads)
        th.join();

    std::vector<Move> bestMoves;
    int maxScore = scores[0];
    for (int i = 1; i < totalMoves; i++)
        if (scores[i] > maxScore)
            maxScore = scores[i];
    for (int i = 0; i < totalMoves; i++)
        if (scores[i] == maxScore)
            bestMoves.push_back(allPossibleMoves[i]);
    return bestMoves[rand() % bestMoves.size()];
}

int Engine::minimax(GameState gameState, int depth, int alpha, int beta, bool isMaximizingPlayer) {
    if (depth == 0 || isGameOver(gameState))
        return evaluateBoard(gameState);

    int evalualtion;
    if (isMaximizingPlayer) {
        evalualtion = INT_MIN;
        auto allPossibleMoves = getAllPossibleMoves(gameState);
        if (allPossibleMoves.empty())
            return evaluateBoard(gameState);

        for (Move move: allPossibleMoves) {
            Piece *piece = Chess::getPiece(gameState.pieces, move.fromLocation);
            PieceType type = piece->getType();
            int eval = minimax(movePiece(&gameState, move), depth - 1, alpha, beta, false);
            piece->setType(type);
            evalualtion = std::max(evalualtion, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) break;
        }
    } else {
        evalualtion = INT_MAX;
        auto allPossibleMoves = getAllPossibleMoves(gameState);
        if (allPossibleMoves.empty())
            return evaluateBoard(gameState);

        for (Move move: allPossibleMoves) {
            Piece *piece = Chess::getPiece(gameState.pieces, move.fromLocation);
            PieceType type = piece->getType();
            int eval = minimax(movePiece(&gameState, move), depth - 1, alpha, beta, true);
            piece->setType(type);
            evalualtion = std::min(evalualtion, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) break;
        }
    }

    return evalualtion;
}

GameState Engine::movePiece(GameState *gameState, Move move) {
    GameState newState = {gameState->pieces, gameState->player};

    Piece *fromPiece = Chess::getPiece(newState.pieces, move.fromLocation);
    Piece *toPiece = Chess::getPiece(newState.pieces, move.toLocation);

    Chess::setPiece(newState.pieces, move.toLocation, fromPiece);
    Chess::setPiece(newState.pieces, move.fromLocation, nullptr);

    // cover promotion
    if (fromPiece->getType() == PAWN && (move.toLocation.rank == 0 || move.toLocation.rank == 7))
        fromPiece->setType(QUEEN);

    // cover castling
    if (fromPiece->getType() == KING) {
        if (move.fromLocation.equals(0, 4) && move.toLocation.equals(0, 6)) {
            Chess::setPiece(newState.pieces, {0, 5}, Chess::getPiece(newState.pieces, {0, 7}));
            Chess::setPiece(newState.pieces, {0, 7}, nullptr);
        } else if (move.fromLocation.equals(0, 4) && move.toLocation.equals(0, 2)) {
            Chess::setPiece(newState.pieces, {0, 3}, Chess::getPiece(newState.pieces, {0, 0}));
            Chess::setPiece(newState.pieces, {0, 0}, nullptr);
        } else if (move.fromLocation.equals(7, 4) && move.toLocation.equals(7, 6)) {
            Chess::setPiece(newState.pieces, {7, 5}, Chess::getPiece(newState.pieces, {7, 7}));
            Chess::setPiece(newState.pieces, {7, 7}, nullptr);
        } else if (move.fromLocation.equals(7, 4) && move.toLocation.equals(7, 2)) {
            Chess::setPiece(newState.pieces, {7, 3}, Chess::getPiece(newState.pieces, {7, 0}));
            Chess::setPiece(newState.pieces, {7, 0}, nullptr);
        }
    }

    // cover en passant
    if (fromPiece->getType() == PAWN && toPiece == nullptr) {
        int deltaFile = move.toLocation.file - move.fromLocation.file;

        if (deltaFile != 0)
            Chess::setPiece(newState.pieces, {move.fromLocation.rank, move.toLocation.file}, nullptr);
    }

    newState.player = newState.player == P_WHITE ? P_BLACK : P_WHITE;
    return newState;
}

std::vector<Move> Engine::getAllPossibleMoves(GameState &gameState) {
    std::vector<Move> moves;
    for (Location position: gameState.getPieces(gameState.player)) {
        LinkedList<Location *> validPositions{};
        Chess::getMoves(gameState.pieces, position, validPositions, false, nullptr, nullptr, nullptr);
        for (int i = 0; i < validPositions.size(); ++i) {
            Location *location = validPositions.get(i);

            // check if this move will cause king capture4
            Move move{position, *location};
            Piece *piece = Chess::getPiece(gameState.pieces, move.fromLocation);
            PieceType type = piece->getType();
            GameState newState = movePiece(&gameState, move);
            if (Chess::isCheckOn(newState.pieces, Chess::getKingLocation(newState.pieces, gameState.player),
                                 gameState.player == P_WHITE,
                                 nullptr, nullptr, nullptr)) {
                piece->setType(type);
                continue;
            }

            piece->setType(type);
            moves.emplace_back(position, *location);
        }
    }

    return moves;
}

int Engine::evaluateBoard(GameState &gameState) {
    int whiteScore = 0;
    int blackScore = 0;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece *piece = Chess::getPiece(gameState.pieces, {row, col});
            if (piece != nullptr) {
                int value = 0;
                switch (piece->getType()) {
                    case KING:
                        value = 200;
                        break;
                    case QUEEN:
                        value = 9;
                        break;
                    case BISHOP:
                    case KNIGHT:
                        value = 5;
                        break;
                    case ROOK:
                        value = 3;
                        break;
                    case PAWN:
                        value = 1;
                        break;
                }

                if (piece->getColor() == P_WHITE)
                    whiteScore += value;
                if (piece->getColor() == P_BLACK)
                    blackScore += value;
            }
        }
    }

    if (gameState.player == P_BLACK)
        return whiteScore - blackScore;

    return blackScore - whiteScore;
}

// TODO: add logic if statelemate
bool Engine::isGameOver(GameState &state) {
    for (auto &pieceRow: state.pieces) {
        for (auto &piece: pieceRow) {
            if (piece != nullptr && piece->getType() == KING &&
                piece->getColor() != state.player)
                return false;
        }
    }
    return true;
}

GameState::GameState(Piece *pieces[8][8], P_Color player) : player(player) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            this->pieces[i][j] = pieces[i][j];
        }
    }
}

std::vector<Location> GameState::getPieces(P_Color _player) {
    std::vector<Location> positions;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece *piece = Chess::getPiece(pieces, {i, j});
            if (piece != nullptr && piece->getColor() == _player)
                positions.emplace_back(i, j);
        }
    }
    return positions;
}
