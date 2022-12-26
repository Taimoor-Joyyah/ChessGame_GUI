#include "Menu.h"
#include "Chess.h"
#include "Popup.h"
#include "Logo.h"

using namespace std;

int main(int argc, char *argv[]) {
    Logo chessLogo{};
    chessLogo.show();

    Menu mainMenu{"MAIN MENU", new string[]{
            "Continue",
            "New Game",
            "Help",
            "Credit",
            "Exit"
    }, 5, true, &chessLogo.getFrame()};

    Menu exitMenu{"EXIT GAME", new string[]{
            "No",
            "Yes"
    }, 2, true, &chessLogo.getFrame()};

    Popup help{"../Help.txt"};
    Popup credit{"../Credit.txt"};

    while (true) {
        switch (mainMenu.selectOption()) {
            case 0: {
                Chess game{true};
                game.startGame();
            }
                break;
            case 1: {
                Chess game{false};
                game.startGame();
            }
                break;
            case 2:
                help.pop();
                break;
            case 3:
                credit.pop();
                break;
            case -1:
            case 4:
                if (exitMenu.selectOption() == 1)
                    exit(0);
        }
    }
}