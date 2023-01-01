//
// Created by J-Bros on 1/1/2023.
//

#ifndef CHESSGAME_GUI_CHESSWINDOW_H
#define CHESSGAME_GUI_CHESSWINDOW_H

#include "raylib.h"
#include "Chess.h"
//#include "LinkedList.h"

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
    static void unloadTexture();
    static void loadTextures();
    static int getTextureIndex(P_Color player, PieceType type);
};

#endif //CHESSGAME_GUI_CHESSWINDOW_H
