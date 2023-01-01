//
// Created by J-Bros on 12/21/2022.
//

#include "Ending.h"
#include "ChessWindow.h"

Ending::Ending(const string *text) : text(text) {}

void Ending::pop() {
    ChessWindow::ending = this;
    int key;
    do {
        key = GetKeyPressed();
    } while (key != KEY_KP_5 && key != KEY_ESCAPE);
    ChessWindow::ending = nullptr;
}

void Ending::setText(const string text[2]) {
    this->text = text;
}
