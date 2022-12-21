//
// Created by J-Bros on 12/17/2022.
//

#ifndef CHESSGAME_FRAME_H
#define CHESSGAME_FRAME_H


class Frame {
public:
    static const int FRAME_X = 41;
    static const int FRAME_Y = 21;
    static const int OPTIMIZED_FRAME_SIZE = FRAME_Y * (FRAME_X + 1);

    Frame();
    void update(int ch, int x, int y);
    char getChar(int x, int y);
    void updateDisplay();
    void copy(Frame &other);
    void clear();
    void createRectangle(int left, int right, int top, int bottom);

private:
    char frame[OPTIMIZED_FRAME_SIZE]{};

    void setupFrame();
};


#endif //CHESSGAME_FRAME_H
