//
// Created by J-Bros on 12/21/2022.
//

#ifndef CHESSGAME_ENDING_H
#define CHESSGAME_ENDING_H

#include <iostream>

using namespace std;

class Ending {
public:
    const string *text;

    Ending(const string *text);
    void setText(const string *text);
    void pop();
};


#endif //CHESSGAME_ENDING_H
