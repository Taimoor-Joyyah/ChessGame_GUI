//
// Created by J-Bros on 12/21/2022.
//

#include "Ending.h"
#include "Input.h"
#include "Enum.h"

Ending::Ending(const string *text, int countLine, Frame *base) : text(text), countLine(countLine), base(base) {}

void Ending::pop() {
    frame.copy(*base);
    setupFrame();
    frame.updateDisplay();
    int key;
    do {
        key = Input::getKey();
    } while (key != Key::SELECT && key != Key::ESC);
}

void Ending::setupFrame() {
    frame.createRectangle(7, 33, 7, 12);

    for (int y = 8; y <= 11; ++y)
        for (int x = 8; x <= 32; ++x)
            frame.update(32, x, y);

    for (int line = 0; line < countLine; ++line)
        for (int i = 0; i < text[line].size(); ++i) {
            int startX = 20 - text[line].size() / 2;
            frame.update(text[line][i], startX + 1 + i, line + 8);
        }
}
