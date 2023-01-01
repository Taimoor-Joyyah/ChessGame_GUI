//
//
// Created by J-Bros on 1/1/2023.

#include "ChessWindow.h"

bool ChessWindow::shouldWindowClose = false;
Texture2D ChessWindow::textures[];
Chess *ChessWindow::game = nullptr;
Menu *ChessWindow::menu = nullptr;
Popup *ChessWindow::popup = nullptr;
Ending *ChessWindow::ending = nullptr;


void ChessWindow::drawWindow() {
    InitWindow(640, 640, "CHESS GAME");
    SetTargetFPS(25);
    SetExitKey(KEY_F4);
    loadTextures();

    Color menuColor{0, 0, 0, 192};
    Color current{0, 228, 48, 128}; // green
    Color selected{230, 41, 55, 128}; // red
    Color possible{0, 121, 241, 128}; // blue
    Color header = ColorAlpha(ORANGE, 0.6);

    while (!WindowShouldClose() && !shouldWindowClose) {
        BeginDrawing();
        ClearBackground(BLANK);

        DrawTexture(textures[(game != nullptr ? 12 : 13)], 0, 0, WHITE);

        if (game != nullptr) {
            DrawRectangle(64, 10, 128, 48, header);
            DrawRectangle(70, game->isWhite ? 12 : 32, 116, 22, RED);
            DrawText("WHITE", 80, 15, 18, WHITE);
            DrawText(to_string(game->whitePoints).c_str(), 160, 15, 20, WHITE);
            DrawText("BLACK", 80, 35, 18, WHITE);
            DrawText(to_string(game->blackPoints).c_str(), 160, 35, 20, WHITE);

            DrawRectangle(448, 10, 128, 45, header);
            DrawText(game->timeToString(game->timePassed).c_str(), 460, 20, 28, WHITE);


            if (game->selectedCell.rank != -1) {
                DrawCircle(96 + 64 * game->selectedCell.file, 96 + 64 * game->selectedCell.rank, 26, selected);
                Piece *piece = game->pieces[game->selectedCell.rank][game->selectedCell.file];
                if (!game->expertMode) {
                    piece->getLegalMoves().iteratorReset();
                    while (!piece->getLegalMoves().isIteratorEnd()) {
                        Location *cell = piece->getLegalMoves().iteratorNext();
                        DrawCircle(96 + 64 * cell->file, 96 + 64 * cell->rank, 18, possible);
                    }
                }
            }

            DrawCircle(96 + 64 * game->currentCell.file, 96 + 64 * game->currentCell.rank, 22, current);
            for (int rank = 0; rank < 8; ++rank) {
                for (int file = 0; file < 8; ++file) {
                    Piece *piece = game->pieces[rank][file];
                    if (piece != nullptr)
                        DrawTexture(textures[getTextureIndex(piece->getColor(), piece->getType())], 70 + 64 * file,
                                    70 + 64 * rank, WHITE);
                }
            }
        }

        if (menu != nullptr) {
            DrawRectangle(160, 160, 320, 70 + menu->optionCount * 50, menuColor);
            DrawText(menu->prompt.c_str(), 320 - (20 * menu->prompt.size() / 2), 180, 32, WHITE);
            for (int i = 0; i < menu->optionCount; ++i) {
                DrawRectangle(180, 220 + i * 50, 280, 40, i == menu->currentOption ? WHITE : GRAY);
                DrawText(menu->options[i].c_str(), 320 - (12 * menu->options[i].size() / 2), 230 + i * 50, 24, BLACK);
            }
        }

        if (popup != nullptr) {
            DrawRectangle(80, 80, 480, 480, menuColor);
            popup->statements.iteratorReset();
            for (int i = 0; i < popup->currentTopLine; ++i)
                popup->statements.iteratorNext();
            for (int i = 0; i < popup->statements.size() && i < 17; ++i) {
                string *statement = popup->statements.iteratorNext();
                DrawText(statement->c_str(), 320 - (12 * statement->size() / 2), 100 + i * 25, 22, YELLOW);
            }
        }

        if (ending != nullptr) {
            DrawRectangle(160, 200, 320, 120, menuColor);
            DrawText(ending->text[0].c_str(), 320 - (24 * ending->text[0].size() / 2), 220, 36, RED);
            DrawText(ending->text[1].c_str(), 320 - (20 * ending->text[0].size() / 2), 260, 30, YELLOW);
        }

        EndDrawing();
    }
    close();
}

void ChessWindow::close() {
    unloadTexture();
    CloseWindow();
}

void ChessWindow::unloadTexture() {
    for (const auto &texture: textures) {
        UnloadTexture(texture);
    }
}

int ChessWindow::getTextureIndex(P_Color player, PieceType type) {
    int index = player == P_WHITE ? 0 : 6;

    switch (type) {
        case KING:
            return index;
        case QUEEN:
            return index + 1;
        case BISHOP:
            return index + 2;
        case KNIGHT:
            return index + 3;
        case ROOK:
            return index + 4;
        case PAWN:
            return index + 5;
    }
    return -1;
}

void ChessWindow::loadTextures() {
    textures[0] = LoadTexture("../res/kingWhite.png");
    textures[1] = LoadTexture("../res/queenWhite.png");
    textures[2] = LoadTexture("../res/bishopWhite.png");
    textures[3] = LoadTexture("../res/knightWhite.png");
    textures[4] = LoadTexture("../res/rookWhite.png");
    textures[5] = LoadTexture("../res/pawnWhite.png");

    textures[6] = LoadTexture("../res/kingBlack.png");
    textures[7] = LoadTexture("../res/queenBlack.png");
    textures[8] = LoadTexture("../res/bishopBlack.png");
    textures[9] = LoadTexture("../res/knightBlack.png");
    textures[10] = LoadTexture("../res/rookBlack.png");
    textures[11] = LoadTexture("../res/pawnBlack.png");

    textures[12] = LoadTexture("../res/boardTex.png");
    textures[13] = LoadTexture("../res/background.png");
}
