//
// Created by J-Bros on 12/26/2022.
//

#ifndef CHESSGAME_LOGO_H
#define CHESSGAME_LOGO_H

#include "Frame.h"
#include "LinkedList.h"

class Logo {
public:
    Logo();

    void show();

    Frame &getFrame();
private:
    Frame frame;
    struct Coordinate;
    LinkedList<Coordinate*> coordinates{};
    void setupFrame();
    void addBlock(int x, int y);
    void updateFrame(int x, int y);
};


#endif //CHESSGAME_LOGO_H
