//
// Created by J-Bros on 12/17/2022.
//

#include "Frame.h"
#include <iostream>

Frame::Frame() {
    setupFrame();
}

void Frame::update(int ch, int x, int y) {
    frame[y * (FRAME_X + 1) + x] = (char) ch;
}

char Frame::getChar(int x, int y) {
    return frame[y * (FRAME_X + 1) + x];
}

void Frame::updateDisplay() {
    std::cout << "\033[1;1H" << frame;
}

void Frame::setupFrame() {
    for (int y = 0; y < FRAME_Y; ++y) {
        for (int x = 0; x < FRAME_X; ++x)
            update(' ', x, y);
        update('\n', FRAME_X, y);
    }
    update('\0', FRAME_X, FRAME_Y - 1);

    for (int x = 0; x < FRAME_X; ++x) {
        update(205, x, 0);
        update(205, x, FRAME_Y - 1);
    }

    for (int y = 0; y < FRAME_Y; ++y) {
        update(186, 0, y);
        update(186, FRAME_X - 1, y);
    }

    update(201, 0, 0);
    update(187, FRAME_X - 1, 0);
    update(200, 0, FRAME_Y - 1);
    update(188, FRAME_X - 1, FRAME_Y - 1);
}

void Frame::clear() {
    setupFrame();
}

void Frame::copy(Frame &other) {
    for (int y = 0; y < Frame::FRAME_Y; ++y) {
        for (int x = 0; x < Frame::FRAME_X; ++x) {
            update(other.getChar(x,y),x,y);
        }
    }
}
