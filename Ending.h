//
// Created by J-Bros on 12/21/2022.
//

#ifndef CHESSGAME_ENDING_H
#define CHESSGAME_ENDING_H

#include <iostream>
#include "Frame.h"

using namespace std;

class Ending {
public:
    Ending(const string *text, int countLine, Frame *base);
    void pop();

private:
    Frame frame;
    Frame *base;
    const string *text;
    int countLine;

    void setupFrame();
};


#endif //CHESSGAME_ENDING_H
