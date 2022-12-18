//
// Created by J-Bros on 12/17/2022.
//

#include "Input.h"
#include <conio.h>

int Input::getKey() {
    return getch();
}

int Input::getIfPressedKey() {
    return _kbhit() ? _getch() : 0;
}