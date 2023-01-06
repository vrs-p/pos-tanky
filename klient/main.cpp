#include <iostream>
#include "application/Application.h"
#include "windows/Menu.h"
#include "windows/ShowScore.h"


int main(int argc, char* args[]) {
    Menu* menu = new Menu();
    menu->render();

    if (!menu->getAppClosed()) {
        Application* app = new Application();
        app->run(menu->getIpAddress(), menu->getPort(), menu->getName());
        ShowScore* showScore = new ShowScore(app->getPlayerScore(), app->getOthersTanks());
        showScore->render();
        delete app;
        delete showScore;
    }

    delete menu;
    return 0;
}