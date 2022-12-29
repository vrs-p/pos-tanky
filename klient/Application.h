#pragma once
//
// Created by vrsp on 28. 12. 2022.
//

#include <SDL.h>
#include <iostream>
#include <list>
#include <thread>
#include "Tank.h"
//#include <SDL_image.h>

#ifndef KLIENT_APPLICATION_H
#define KLIENT_APPLICATION_H

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

class Application {
public:
    Application();
    ~Application();


    void render();
    void readClientInput();
    void checkBorders();
    void update();
    void draw();
    void initializeWindow();

    void run();

private:
    bool isRunning;

    Tank* clientTank_;
    std::list<Tank*>* otherTanks;

    SDL_Window *window;
    SDL_Surface *windowSurface;
    SDL_Event windowEvent;
    SDL_Renderer *renderer;
};


#endif //KLIENT_APPLICATION_H
