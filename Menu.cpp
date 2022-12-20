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
    for (int y = 5; y <= 15; ++y)
        for (int x = 11; x < 30; ++x)
            frame.update(x % 2 ? 250 : 32, x, y);

    for (int i = 0; i < prompt.size(); ++i) {
        int startX = 20 - prompt.size() / 2;
        frame.update(prompt[i], startX + 1 + i, 6);
    }

    for (int option = 0; option < optionCount; ++option)
        for (int i = 0; i < options[option].size(); ++i) {
            int startX = 20 - options[option].size() / 2;
            frame.update(options[option][i], startX + 1 + i, option + 8);
        }

    for (int x = 11; x < 30; ++x) {
        frame.update(205, x, 5);
        frame.update(205, x, 15);
    }

    for (int y = 5; y <= 15; ++y) {
        frame.update(186, 11, y);
        frame.update(186, 29, y);
    }

    frame.update(201, 11, 5);
    frame.update(187, 29, 5);
    frame.update(200, 11, 15);
    frame.update(188, 29, 15);
}

void Menu::updateFrame(int currentOption, int previousOption) {
    int curSize = options[currentOption].size();
    int prevSize = options[previousOption].size();

    for (int x = 12; x < 29; ++x) {
        int startXPrev = 20 - prevSize / 2;
        if (x <= startXPrev || x > startXPrev + prevSize)
            frame.update(x % 2 ? 250 : 32, x, previousOption + 8);

        int startXCur = 20 - curSize / 2;
        if (x <= startXCur || x > startXCur + curSize)
            frame.update(219, x, currentOption + 8);
    }
}
