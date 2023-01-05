#include <iostream>

#include "Application.h"
#include "Menu.h"
#include<SFML/Network.hpp>

//const int SCREEN_WIDTH = 800;
//const int SCREEN_HEIGHT = 800;

int main(int argc, char* args[]) {
    Application* app = new Application();
    Menu* menu = new Menu();
    menu->render();
    app->run(menu->getIpAddress(), menu->getPort());
    app->printScore();

    delete menu;
    delete app;
    return 0;
}