#include "Menu.h"
#include "Chess.h"
#include "Popup.h"
#include "ChessWindow.h"

#include <thread>

using namespace std;

int main(int argc, char *argv[]) {
    Menu mainMenu{"MAIN MENU", new string[]{
            "Continue",
            "New Game",
            "Help",
            "Credit",
            "Exit"
    }, 5, true};

    Menu exitMenu{"EXIT GAME", new string[]{
            "No",
            "Yes"
    }, 2, true};

    Popup help{"../Help.txt"};
    Popup credit{"../Credit.txt"};

    thread window(ChessWindow::drawWindow);

    while (true) {
        switch (mainMenu.selectOption()) {
            case 0: {
                if (FileExists("prevState.sav")) {
                    Chess game{true};
                    game.startGame();
                }
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
                if (exitMenu.selectOption() == 1) {
                    ChessWindow::shouldWindowClose = true;
                    window.join();
                    exit(0);
                }
        }
    }
}