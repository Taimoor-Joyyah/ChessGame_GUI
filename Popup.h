//
// Created by J-Bros on 12/20/2022.
//

#ifndef CHESSGAME_POPUP_H
#define CHESSGAME_POPUP_H

#include <iostream>
#include "LinkedList.h"
#include "Frame.h"

using namespace std;

class Popup {
public:
    explicit Popup(const string filename);
    void pop();

private:
    LinkedList<string*> statements{};
    Frame frame;

    void setupFrame();
    void updateFrame(int topLine);
    void setStatements(const string filename);
    void clear();
};


#endif //CHESSGAME_POPUP_H
