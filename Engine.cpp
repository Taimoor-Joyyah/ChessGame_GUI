//
// Created by mtaimoor on 9/8/2023.
//

#include "Engine.h"
#include <limits>
#include <vector>

Piece ***Engine::pieceList = nullptr;
P_Color *Engine::currentPlayer = nullptr;
int Engine::power = 0;

void Engine::setEngine(Piece ***pieceList, P_Color *currentPlayer, int power) {
    Engine::pieceList = pieceList;
    Engine::currentPlayer = currentPlayer;
    Engine::power = power;
}

void Engine::setPower(int power) {
    Engine::power = power;
}

//Move Engine::evaluateBestMove() {
//    if (pieceList == nullptr || currentPlayer == nullptr) {
//        return Move{nullptr, P_WHITE};
//    }
//
//    std::vector<Move> allPossibleMoves = getAllPossibleMoves(gameState, this);
//    int totalMoves = allPossibleMoves.size();
//    std::vector<int> scores(totalMoves);
//
//    std::vector<std::thread> threads;
//    std::mutex firstCheck;
//    for (int i = 0; i < totalMoves; i++) {
//        threads.push_back(std::thread([&, i] {
//            scores[i] = minimax(movePiece(gameState, allPossibleMoves[i]), depth - 1, INT_MIN, INT_MAX, false);
//        }));
//    }
//    for (auto &th: threads) th.join();
//
//    std::vector<Move> bestMoves;
//    int maxScore = *std::max_element(scores.begin(), scores.end());
//    for (int i = 0; i < totalMoves; i++) {
//        if (scores[i] == maxScore) {
//            bestMoves.push_back(allPossibleMoves[i]);
//        }
//    }
//
//    return bestMoves;
//}
//
//int Engine::minimax(GameState gameState, int depth, int alpha, int beta, bool isMaximizingPlayer) {
//    if (depth == 0 || gameState.isGameOver()) {
//        return evaluateBoard(gameState);
//    }
//
//    int evalualtion;
//    if (isMaximizingPlayer) {
//        evalualtion = INT_MIN;
//        for (Move move: getAllPossibleMoves(gameState, gameState.currentPlayer())) {
//            int eval = minimax(movePiece(gameState, move), depth - 1, alpha, beta, false);
//            evalualtion = std::max(evalualtion, eval);
//            alpha = std::max(alpha, eval);
//            if (beta <= alpha) break;
//        }
//    } else {
//        evalualtion = INT_MAX;
//        for (Move move: getAllPossibleMoves(gameState, gameState.currentPlayer())) {
//            int eval = minimax(movePiece(gameState, move), depth - 1, alpha, beta, true);
//            evalualtion = std::min(evalualtion, eval);
//            beta = std::min(beta, eval);
//            if (beta <= alpha) break;
//        }
//    }
//
//    return evalualtion;
//}
//
//
//GameState Engine::movePiece(GameState gameState, Move move) {
//    GameState newState = gameState.clone(gameState.currentPlayer(), gameState.opponentPlayer());
//    Piece piece = gameState.getPiece(move.fromRow(), move.fromCol());
//    newState.setPiece(move.fromRow(), move.fromCol(), NULL);
//    newState.setPiece(move.toRow(), move.toCol(), piece);
//    newState.nextPlayer();
//    return newState;
//}
//
//
//std::vector<Move> Engine::getAllPossibleMoves(GameState gameState, P_Color *player) {
//    std::vector<Move> moves;
//    for (GameState::PiecePosition piecePosition: gameState.getPieces(player)) {
//        std::vector<Move> tmpMoves = gameState.getAllValidMove(piecePosition.piece(), piecePosition.row(),
//                                                               piecePosition.col());
//        moves.insert(moves.end(), tmpMoves.begin(), tmpMoves.end());
//    }
//    return moves;
//}
//
//
//int Engine::evaluateBoard(GameState gameState) {
//    int score = 0;
//    std::vector<int> pieceValue = {200, 9, 3, 3, 5, 1};
//    for (int row = 1; row <= 8; row++) {
//        for (int col = 1; col <= 8; col++) {
//            Piece *piece = gameState.getPiece(row, col);
//            if (piece != nullptr) {
//                int value = pieceValue[piece->getType().ordinal()];
//                score += (piece->getPlayer() == this) ? value : -value;
//            }
//        }
//    }
//    return score;
//}