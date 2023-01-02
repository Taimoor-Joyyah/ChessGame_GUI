//
//
// Created by J-Bros on 12/17/2022.

#include "Menu.h"
#include "raylib.h"
#include "ChessWindow.h"

Menu::Menu(string &&prompt, string *options, int optionCount, bool escapable)
        : prompt(prompt), options(options), optionCount(optionCount), escapable(escapable), currentOption(0) {
}

int Menu::selectOption() {
    currentOption = 0;
    ChessWindow::menu = this;
    int key{};
    do {
        key = GetKeyPressed();
        switch (key) {
            case KEY_KP_8:
                currentOption = currentOption == 0 ? optionCount - 1 : currentOption - 1;
                break;
            case KEY_KP_2:
                currentOption = (currentOption + 1) % optionCount;
                break;
            case KEY_ESCAPE:
                if (escapable)
                    currentOption = -1;
        }
    } while (key != KEY_KP_5 && (!escapable || key != KEY_ESCAPE));
    ChessWindow::menu = nullptr;
    return currentOption;
}