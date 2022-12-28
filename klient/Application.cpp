//
// Created by vrsp on 28. 12. 2022.
//

#include "Application.h"

Application::Application() {
    this->initializeWindow();

    this->clientTank_ = new Tank();
    this->clientTank_->getIcon()->x = (SCREEN_WIDTH - tank->w) / 2;
    this->clientTank_->getIcon()->y = (SCREEN_HEIGHT - tank->w) / 2;

}

Application::~Application() {
    SDL_FreeSurface(windowSurface);
    SDL_DestroyWindow(window);
}

int Application::loop() {
    int close = 0;

    while (!close) {
        close = this->readClientInput();
        this->checkBorders();
        this->draw();
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
    this->clientTank_->render(*renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000 / 60);
}

int Application::readClientInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {

            case SDL_QUIT:
                return 1;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        this->clientTank_->moveUp();
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        this->clientTank_->moveLeft();
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        this->clientTank_->moveDown();
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        this->clientTank_->moveRight();
                        break;
                    default:
                        break;
                }
        }
    }
    return 0;
}

void Application::checkBorders() {
    if (this->clientTank_->getIcon()->x + this->clientTank_->getIcon()->w > SCREEN_WIDTH)
        this->clientTank_->getIcon()->x = SCREEN_WIDTH - this->clientTank_->getIcon()->w;

    if (this->clientTank_->getIcon()->x < 0)
        this->clientTank_->getIcon()->x = 0;

    if (this->clientTank_->getIcon()->y + this->clientTank_->getIcon()->h > SCREEN_HEIGHT)
        this->clientTank_->getIcon()->y = SCREEN_HEIGHT - this->clientTank_->getIcon()->h;

    if (this->clientTank_->getIcon()->y < 0)
        this->clientTank_->getIcon()->y = 0;
}

void Application::initializeWindow() {
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
