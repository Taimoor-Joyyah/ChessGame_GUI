//
// Created by J-Bros on 12/20/2022.
//

#include <fstream>
#include "Popup.h"
#include "Enum.h"
#include "Input.h"

Popup::Popup(const string filename) {
    setStatements(filename);
}


void Popup::pop() {
    setupFrame();
    frame.updateDisplay();
    int currentTopLine = 0;
    int key;
    do {
        key = Input::getKey();

        switch (key) {
            case Key::UP:
                if (currentTopLine > 0)
                    --currentTopLine;
                break;
            case Key::DOWN:
                if (currentTopLine < statements.size() - 17)
                    ++currentTopLine;
                break;
            default:
                continue;
        }
        updateFrame(currentTopLine);
        frame.updateDisplay();
    } while (key != Key::SELECT && key != Key::ESC);
}


void Popup::setupFrame() {
    for (int x = 1; x <= 39; ++x) {
        frame.update(196, x, 1);
        frame.update(196, x, 19);
    }
    frame.update(239, 20, 1);
    frame.update('V', 20, 19);

    updateFrame(0);
}

void Popup::updateFrame(int topLine) {
    statements.iteratorReset();
    for (int i = 0; i < topLine; ++i)
        statements.iteratorNext();
    clear();
    for (int i = 0; i < statements.size() && i < 17; ++i) {
        string *statement = statements.iteratorNext();
        for (int j = 0; j < statement->size(); ++j) {
            int startX = 20 - statement->size() / 2;
            frame.update((*statement)[j], startX + 1 + j, i + 2);
        }
    }
}

void Popup::setStatements(const string filename) {
    ifstream file{filename};
    string statement;
    if (file.is_open()) {
        while (getline(file, statement))
            statements.insert(new string{statement});
        file.close();
    }
}

void Popup::clear() {
    for (int y = 2; y < 19; ++y)
        for (int x = 1; x <= 39; ++x)
            frame.update(32, x, y);
}

