//
// Created by J-Bros on 1/1/2023.
//

#ifndef CHESSGAME_GUI_CHESSWINDOW_H
#define CHESSGAME_GUI_CHESSWINDOW_H

#include "raylib.h"
#include "Chess.h"

class ChessWindow {
public:
    static Chess *game;
    static Menu *menu;
    static Popup *popup;
    static Ending *ending;

    static Texture2D textures[14];
    static bool shouldWindowClose;

    static void drawWindow();

    static void close();

private:
    static string timeToString(int seconds);

    static void unloadTexture();

    static void loadTextures();

    static int getTextureIndex(P_Color player, PieceType type);

    static void drawMenu(Color &menuColor);

    static void drawPopup(Color &menuColor);

    static void drawEnding(Color &menuColor);

    static void drawChess(Color &current, Color &selected, const Color &possible, const Color &capture, Color &header);

    static void drawBackground();

    static void drawHeaders(Color &header);

    static void drawCellGuides(Color &current, Color &selected, const Color &possible, const Color &capture);

    static void drawPieces();
};

#endif //CHESSGAME_GUI_CHESSWINDOW_H
