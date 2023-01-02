//
//
// Created by J-Bros on 1/1/2023.

#include "raylib.h"
#include "Chess.h"
#include <fstream>
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

    Color menuColor = ColorAlpha(BLACK, 0.75);
    Color current = ColorAlpha(GREEN, 0.5);
    Color selected = ColorAlpha(RED, 0.5);
    Color possible = ColorAlpha(BLUE, 0.5);
    Color capture = ColorAlpha(PURPLE, 0.75);
    Color header = ColorAlpha(ORANGE, 0.6);

    while (!WindowShouldClose() && !shouldWindowClose) {
        BeginDrawing();
        ClearBackground(BLANK);

        drawBackground();
        drawChess(current, selected, possible, capture, header);
        drawMenu(menuColor);
        drawPopup(menuColor);
        drawEnding(menuColor);

        EndDrawing();
    }
    close();
}

void ChessWindow::drawBackground() {
    DrawTexture(textures[(game != nullptr ? 12 : 13)], 0, 0, WHITE);
}

void ChessWindow::drawChess(Color &current, Color &selected,
                            const Color &possible, const Color &capture, Color &header) {
    if (game != nullptr) {
        drawHeaders(header);
        drawCellGuides(current, selected, possible, capture);
        drawPieces();
    }
}

void ChessWindow::drawPieces() {
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            Piece *piece = game->pieces[rank][file];
            if (piece != nullptr)
                DrawTexture(textures[getTextureIndex(piece->getColor(), piece->getType())], 70 + 64 * file,
                            70 + 64 * rank, WHITE);
        }
    }
}

void ChessWindow::drawCellGuides(Color &current, Color &selected, const Color &possible, const Color &capture) {
    if (game->selectedCell.rank != -1) {
        DrawCircle(96 + 64 * game->selectedCell.file, 96 + 64 * game->selectedCell.rank, 26, selected);
        Piece *piece = game->pieces[game->selectedCell.rank][game->selectedCell.file];
        if (!game->expertMode) {
            piece->getLegalMoves().iteratorReset();
            while (!piece->getLegalMoves().isIteratorEnd()) {
                Location *cell = piece->getLegalMoves().iteratorNext();
                DrawCircle(96 + 64 * cell->file, 96 + 64 * cell->rank, 18,
                           game->pieces[cell->rank][cell->file] != nullptr ? capture : possible);
            }
        }
    }
    DrawCircle(96 + 64 * game->currentCell.file, 96 + 64 * game->currentCell.rank, 22, current);
}

void ChessWindow::drawHeaders(Color &header) {
    DrawRectangle(64, 10, 128, 48, header);
    DrawRectangle(70, game->isWhite ? 12 : 32, 116, 22, RED);
    DrawText("WHITE", 80, 15, 18, WHITE);
    DrawText(to_string(game->whitePoints).c_str(), 160, 15, 20, WHITE);
    DrawText("BLACK", 80, 35, 18, WHITE);
    DrawText(to_string(game->blackPoints).c_str(), 160, 35, 20, WHITE);

    DrawRectangle(448, 10, 128, 45, header);
    DrawText(timeToString(game->timePassed).c_str(), 460, 20, 28, WHITE);
}

void ChessWindow::drawEnding(Color &menuColor) {
    if (ending != nullptr) {
        DrawRectangle(160, 200, 320, 120, menuColor);
        DrawText(ending->text[0].c_str(), 320 - (24 * ending->text[0].size() / 2), 220, 36, RED);
        DrawText(ending->text[1].c_str(), 320 - (20 * ending->text[0].size() / 2), 260, 30, YELLOW);
    }
}

void ChessWindow::drawPopup(Color &menuColor) {
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
}

void ChessWindow::drawMenu(Color &menuColor) {
    if (menu != nullptr) {
        DrawRectangle(160, 160, 320, 70 + menu->optionCount * 50, menuColor);
        DrawText(menu->prompt.c_str(), 320 - (20 * menu->prompt.size() / 2), 180, 32, WHITE);
        for (int i = 0; i < menu->optionCount; ++i) {
            DrawRectangle(180, 220 + i * 50, 280, 40, i == menu->currentOption ? WHITE : GRAY);
            DrawText(menu->options[i].c_str(), 320 - (12 * menu->options[i].size() / 2), 230 + i * 50, 24, BLACK);
        }
    }
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

string ChessWindow::timeToString(int seconds) {
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