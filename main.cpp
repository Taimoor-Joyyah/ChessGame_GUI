#include "Menu.h"
#include "Chess.h"
#include "Popup.h"

using namespace std;

int main(int argc, char *argv[]) {
    Menu mainMenu{"MAIN MENU", new string[]{
            "Start Game",
            "Help",
            "Credit",
            "Exit"
    }, 4, true};

    Menu exitMenu{"EXIT GAME", new string[]{
            "No",
            "Yes"
    }, 2, true};

    Popup help{"../Help.txt"};
    Popup credit{"../Credit.txt"};

    while (true) {
        switch (mainMenu.selectOption()) {
            case 0: {
                Chess game{};
                game.startGame();
            }
                break;
            case 1:
                help.pop();
                break;
            case 2:
                credit.pop();
                break;
            case -1:
            case 3:
                if (exitMenu.selectOption() == 1)
                    exit(0);
        }
    }
}