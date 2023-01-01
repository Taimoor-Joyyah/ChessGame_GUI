//
// Created by J-Bros on 12/20/2022.
//

#ifndef CHESSGAME_POPUP_H
#define CHESSGAME_POPUP_H

#include <iostream>
#include "LinkedList.h"

using namespace std;

class Popup {
public:
    int currentTopLine{};

    explicit Popup(const string &filename);
    LinkedList<string*> statements{};
    void pop();

private:
    void setStatements(const string &filename);
};


#endif //CHESSGAME_POPUP_H
