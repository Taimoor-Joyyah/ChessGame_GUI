//
// Created by J-Bros on 12/17/2022.
//

#ifndef CHESSGAME_MENU_H
#define CHESSGAME_MENU_H

#include <iostream>
#include "Frame.h"

using namespace std;

class Menu {
public:
    Menu(string &&prompt, string *options, int optionCount, bool escapable);
    Menu(string &&prompt, string *options, int optionCount, bool escapable, Frame *base);
    int selectOption();

private:
    string prompt;
    string *options;
    int optionCount;
    bool escapable;
    Frame frame;
    Frame *base;

    void setupFrame();
    void updateFrame(int currentOption, int previousOption);
};


#endif //CHESSGAME_MENU_H
