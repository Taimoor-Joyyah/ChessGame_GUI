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

    createRectangle(0, FRAME_X - 1, 0, FRAME_Y - 1);
}

void Frame::createRectangle(int left, int right, int top, int bottom) {
    for (int x = left; x <= right; ++x) {
        update(205, x, top);
        update(205, x, bottom);
    }
    for (int y = top; y <= bottom; ++y) {
        update(186, left, y);
        update(186, right, y);
    }
    update(201, left, top);
    update(187, right, top);
    update(200, left, bottom);
    update(188, right, bottom);
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
