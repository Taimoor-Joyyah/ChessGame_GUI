//
// Created by J-Bros on 12/17/2022.
//

#ifndef CHESSGAME_MENU_H
#define CHESSGAME_MENU_H

#include <iostream>

using namespace std;

class Menu {
public:
    string prompt;
    string *options;
    int optionCount;
    int currentOption;

    Menu(string &&prompt, string *options, int optionCount, bool escapable);

    int selectOption();
private:
    bool escapable;
};

#endif //CHESSGAME_MENU_H
