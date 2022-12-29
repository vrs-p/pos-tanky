#pragma once
//
// Created by vrsp on 28. 12. 2022.
//

#include "Tank.h"
#include <SDL.h>
//#include <SDL_image.h>
#include <iostream>
#include "list"

#ifndef KLIENT_APPLICATION_H
#define KLIENT_APPLICATION_H

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

class Application {
public:
    Application();
    ~Application();

    int loop();
    void update();
    void draw();
    int readClientInput();
    void checkBorders();

    void initializeWindow();

private:
    SDL_Rect *tank;
    Tank* clientTank_;
    std::list<Tank*>* otherTanks;

    SDL_Window *window;
    SDL_Surface *windowSurface;
    SDL_Event windowEvent;
    SDL_Renderer *renderer;
};


#endif //KLIENT_APPLICATION_H
