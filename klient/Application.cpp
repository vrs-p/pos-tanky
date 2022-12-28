//
// Created by vrsp on 28. 12. 2022.
//

#include "Application.h"

Application::Application() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    window = SDL_CreateWindow("POS-Tanky", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        printf("Failed to create window: %s\n", SDL_GetError());
        return;
    }

    windowSurface = SDL_GetWindowSurface(window);
    if (!windowSurface) {
        printf("Failed to get window's surface: %s\n", SDL_GetError());
        return;
    }

    SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0x00, 0x00, 0x00));

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Failed to create renderer!\n");
    }

    tank->w = 20;
    tank->h = 40;

    tank->x = (SCREEN_WIDTH - tank->w) / 2;
    tank->y = (SCREEN_HEIGHT - tank->h) / 2;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

Application::~Application() {
    SDL_FreeSurface(windowSurface);
    SDL_DestroyWindow(window);
}

int Application::loop() {
    int close = 0;
    int speed = 100;

    while (!close) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {

                case SDL_QUIT:
                    close = 1;
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            tank->y -= speed / 20;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            tank->x -= speed / 20;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            tank->y += speed / 20;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            tank->x += speed / 20;
                            break;
                        default:
                            break;
                    }
            }
            draw();
        }

        if (tank->x + tank->w > SCREEN_WIDTH)
            tank->x = SCREEN_WIDTH - tank->w;

        if (tank->x < 0)
            tank->x = 0;

        if (tank->y + tank->h > SCREEN_HEIGHT)
            tank->y = SCREEN_HEIGHT - tank->h;

        if (tank->y < 0)
            tank->y = 0;

        draw();

        SDL_Delay(1000 / 60);
    }

    return 0;
}

void Application::update() {
    bool windowOpened = true;
    while (windowOpened) {
        while (SDL_PollEvent(&windowEvent) > 0) {
            switch (windowEvent.type) {
                case SDL_QUIT:
                    windowOpened = false;
                    break;
            }
        }

        draw();
    }
}

void Application::draw() {
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, reinterpret_cast<const SDL_Rect *>(&tank));
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
}