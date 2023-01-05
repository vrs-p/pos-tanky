#include <iostream>

#include "Application.h"
#include "Menu.h"
#include "ShowScore.h"
#include<SFML/Network.hpp>

//const int SCREEN_WIDTH = 800;
//const int SCREEN_HEIGHT = 800;

int main(int argc, char* args[]) {
    Application* app = new Application();
    Menu* menu = new Menu();
    menu->render();
    app->run(menu->getIpAddress(), menu->getPort());
    ShowScore* showScore = new ShowScore(app->getPlayerScore(), app->getOthersTanks());
    showScore->render();

    delete menu;
    delete app;
    delete showScore;
    return 0;
}