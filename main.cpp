#include "Menu.h"
#include "Chess.h"

using namespace std;

int main(int argc, char *argv[]) {
    Menu mainMenu{
            "MAIN MENU",
            new string[]{
                    "Start Game",
                    "Help",
                    "Credit",
                    "Exit"
            },
            4
    };

    Menu exitMenu{
            "EXIT GAME",
            new string[]{
                    "No",
                    "Yes"
            },
            2
    };


    while (true) {
        switch (mainMenu.selectOption()) {
            case 0: {
                Chess game{};
                game.startGame();
            }
                break;
            case 1:
                //... TODO: HELP
                break;
            case 2:
                //... TODO: CREDIT
                break;
            case 3:
                if (exitMenu.selectOption() == 1)
                    exit(0);
        }
    }
}