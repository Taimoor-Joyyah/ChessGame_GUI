//
// Created by J-Bros on 12/26/2022.
//

#include "Logo.h"
#include "Input.h"
#include <unistd.h>
#include <string>

Logo::Logo() {
    setupFrame();
}

struct Logo::Coordinate {
    int x;
    int y;
};

void Logo::setupFrame() {
    for (int y = 1; y <= 19; ++y)
        for (int x = 1; x <= 39; ++x)
            frame.update(x % 2 ? 250 : 32, x, y);
}

void Logo::show() {
    addBlock(20, 2);
    addBlock(19, 2);
    addBlock(18, 2);
    addBlock(17, 3);
    addBlock(16, 4);
    addBlock(16, 5);
    addBlock(17, 6);
    addBlock(18, 7);
    addBlock(17, 8);
    addBlock(16, 9);
    addBlock(15, 10);
    addBlock(14, 11);
    addBlock(15, 11);
    addBlock(16, 11);
    addBlock(17, 11);
    addBlock(18, 11);
    addBlock(19, 11);
    addBlock(20, 11);
    addBlock(14, 12);
    addBlock(14, 13);
    addBlock(15, 13);
    addBlock(16, 13);
    addBlock(17, 13);
    addBlock(18, 13);
    addBlock(19, 13);
    addBlock(20, 13);

    frame.updateDisplay();

    int magnitude = 0;
    while(magnitude < 99) {
        usleep(20000);
        ++magnitude;

        setupFrame();
        coordinates.iteratorReset();
        while (!coordinates.isIteratorEnd()) {
            Coordinate *coordinate = coordinates.iteratorNext();
            updateFrame(coordinate->x, 1 + (coordinate->y * magnitude)/100);
        }
        frame.updateDisplay();
    }

    std::string prompt1 = "PRESS NUM 5";
    std::string prompt2 = "TO CONTINUE";

    for (int i = 0; i < 11; ++i) {
        frame.update(prompt1[i], 15 + i, 15);
        frame.update(prompt2[i], 15 + i, 16);
    }

    frame.updateDisplay();

    while (Input::getKey() != '5');

    for (int x = 1; x <= 39; ++x) {
        frame.update(x % 2 ? 250 : 32, x, 15);
        frame.update(x % 2 ? 250 : 32, x, 16);
    }
}

Frame &Logo::getFrame() {
    return frame;
}

void Logo::addBlock(int x, int y) {
    coordinates.insert(new Coordinate{x, y});
    coordinates.insert(new Coordinate{40 - x, y});
}

void Logo::updateFrame(int x, int y) {
    frame.update(219, x, y);
}
