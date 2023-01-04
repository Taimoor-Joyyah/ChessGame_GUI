//
// Created by J-Bros on 12/20/2022.
//

#include <fstream>
#include "Popup.h"
#include "ChessWindow.h"

Popup::Popup(const string &filename) {
    setStatements(filename);
}

void Popup::pop() {
    currentTopLine = 0;
    ChessWindow::popup = this;

    bool scrolling = false;
    int scroll{};
    int key{};
    do {
        key = GetKeyPressed();

        scroll = GetMouseWheelMove();
        if (scroll) {
            if (!scrolling) {
                key = scroll < 0 ? KEY_KP_2 : KEY_KP_8;
                scrolling = true;
            }
        }
        else
            scrolling = false;

        switch (key) {
            case KEY_KP_8:
                if (currentTopLine > 0)
                    --currentTopLine;
                break;
            case KEY_KP_2:
                if (currentTopLine < statements.size() - 17)
                    ++currentTopLine;
                break;
        }
    } while (key != KEY_KP_5 && key != KEY_ESCAPE);
    ChessWindow::popup = nullptr;
}

void Popup::setStatements(const string &filename) {
    ifstream file{filename};
    string statement;
    if (file.is_open()) {
        while (getline(file, statement))
            statements.insert(new string{statement});
        file.close();
    }
}
