#include "Menu.h"
#include "Chess.h"
#include "Popup.h"
#include "ChessWindow.h"

#include <thread>

using namespace std;

int menuPower(int option) {
    switch (option) {
        case 0:
            return 0;
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        case 4:
            return 5;
    }

    return 0;
}

int main() {
    Menu mainMenu{
        "MAIN MENU", new string[]{
            "Continue",
            "New Game",
            "Help",
            "Credit",
            "Exit"
        },
        5, true
    };

    Menu exitMenu{
        "EXIT GAME", new string[]{
            "No",
            "Yes"
        },
        2, true
    };

    Menu gameTypeMenu{
        "GAME MOD", new string[]{
            "Human vs Human",
            "Human vs CPU",
            "CPU vs CPU",
            "Back"
        },
        4, true
    };

    Menu powerMenu{
        "POWER MENU", new string[]{
            "Random",
            "Easy",
            "Medium",
            "Hard",
            "Expert"
        },
        5, false
    };

    Menu sideSelectMenu{
        "SELECT SIDE", new string[]{
            "White",
            "Black"
        },
        2, false
    };

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
                switch (gameTypeMenu.selectOption()) {
                    case 0: {
                        Chess game{false, 0, P_HUMAN, P_HUMAN, 0, 0};
                        game.startGame();
                        break;
                    }
                    case 1: {
                        P_Type whiteSide;
                        P_Type blackSide;

                        switch (sideSelectMenu.selectOption()) {
                            case 0:
                                whiteSide = P_HUMAN;
                                blackSide = P_CPU;
                                break;
                            case 1:
                                whiteSide = P_CPU;
                                blackSide = P_HUMAN;
                                break;
                        }

                        int power = menuPower(powerMenu.selectOption());

                        Chess game{false, 1, whiteSide, blackSide, power, power};
                        game.startGame();
                        break;
                    }
                    case 2: {
                        int whitePower;
                        int blackPower;

                        powerMenu.prompt = "WHITE POWER";
                        whitePower = menuPower(powerMenu.selectOption());

                        powerMenu.prompt = "BLACK POWER";
                        blackPower = menuPower(powerMenu.selectOption());

                        Chess game{false, 2, P_CPU, P_CPU, whitePower, blackPower};
                        game.startGame();
                        break;
                    }
                }
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
