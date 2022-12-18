//
//
// Created by J-Bros on 12/17/2022.

#include "Menu.h"
#include "Input.h"
#include "Enum.h"

Menu::Menu(string &&prompt, string *options, int optionCount) {
    this->base = nullptr;
    this->prompt = prompt;
    this->options = options;
    this->optionCount = optionCount;
}

Menu::Menu(string &&prompt, string *options, int optionCount, Frame *base)
        : prompt(prompt), options(options), optionCount(optionCount), base(base) {
}

int Menu::selectOption() {
    int previousOption = 0;
    int currentOption = 0;
    if (base) frame.copy(*base);
    setupFrame();
    updateFrame(currentOption, previousOption);
    frame.updateDisplay();
    int key;
    do {
        previousOption = currentOption;
        key = Input::getKey();

        switch (key) {
            case Key::UP:
                currentOption = currentOption == 0 ?
                                optionCount - 1 : currentOption - 1;
                break;
            case Key::DOWN:
                currentOption = (currentOption + 1) % optionCount;
                break;
            default:
                continue;
        }
        updateFrame(currentOption, previousOption);
        frame.updateDisplay();
    } while (key != Key::SELECT);

    return currentOption;
}

void Menu::setupFrame() {
    int startY = 8 - optionCount / 2;

    for (int y = 0; y <= 10; ++y)
        for (int x = 11; x < 30; ++x)
            frame.update(x % 2 ? 250 : 32, x, 5 + y);

    for (int i = 0; i < prompt.size(); ++i) {
        int startX = 20 - prompt.size() / 2;
        frame.update(prompt[i], startX + 1 + i, startY);
    }

    for (int option = 0; option < optionCount; ++option)
        for (int i = 0; i < options[option].size(); ++i) {
            int startX = 20 - options[option].size() / 2;
            frame.update(options[option][i], startX + 1 + i, startY + option + 2);
        }
}

void Menu::updateFrame(int currentOption, int previousOption) {
    int startY = 8 - optionCount / 2;
    int curSize = options[currentOption].size();
    int prevSize = options[previousOption].size();
    int startXCur = 20 - curSize / 2;
    int startXPrev = 20 - prevSize / 2;

    for (int x = 11; x < 30; ++x) {
        if (x <= startXPrev || x > startXPrev + prevSize)
            frame.update(x % 2 ? 250 : 32, x, startY + previousOption + 2);
        if (x <= startXCur || x > startXCur + curSize)
            frame.update(219, x, startY + currentOption + 2);
    }
}
