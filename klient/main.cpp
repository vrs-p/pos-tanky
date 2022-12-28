#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "Application.h"

//const int SCREEN_WIDTH = 800;
//const int SCREEN_HEIGHT = 800;

int main(int argc, char* args[]) {

//    // returns zero on success else non-zero
//    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
//        printf("error initializing SDL: %s\n", SDL_GetError());
//    }
//    SDL_Window *window = SDL_CreateWindow("POS-Tanky", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
//                                          SCREEN_WIDTH, SCREEN_HEIGHT, 0);
//    if (window == NULL) {
//        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
//    }
//
//    SDL_Surface *screenSurface = SDL_GetWindowSurface(window);
//
//    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
//
////    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
////        printf("Failed to initialise SDL_image for PNG files: %s\n", IMG_GetError());
////    }
//
//    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//    if (renderer == NULL) {
//        printf("Failed to create renderer!\n");
//    }
//
////    SDL_Surface* surface_tank = IMG_Load("img/dalle_croppedTank.png");
////    if (surface_tank == NULL) {
////        printf("Failed to load image: %s\n", IMG_GetError());
////    }
////
////    SDL_Texture* texture_tank = SDL_CreateTextureFromSurface(renderer, surface_tank);
////    if (texture_tank == NULL) {
////        printf("Failed to create texture!\n");
////    }
////
////    SDL_FreeSurface(surface_tank);
//
//    SDL_Rect tank;
//
//    tank.w = 20;
//    tank.h = 40;
//
//    tank.x = (SCREEN_WIDTH - tank.w) / 2;
//    tank.y = (SCREEN_HEIGHT - tank.h) / 2;
//
//    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//    SDL_RenderDrawRect(renderer, &tank);
//
//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//
//    SDL_RenderPresent(renderer);
//
//    int close = 0;
//    int speed = 100;
//
//    while (!close) {
//        SDL_Event event;
//
//        while (SDL_PollEvent(&event)) {
//            switch (event.type) {
//
//                case SDL_QUIT:
//                    close = 1;
//                    break;
//
//                case SDL_KEYDOWN:
//                    switch (event.key.keysym.scancode) {
//                        case SDL_SCANCODE_W:
//                        case SDL_SCANCODE_UP:
//                            tank.y -= speed / 20;
//                            break;
//                        case SDL_SCANCODE_A:
//                        case SDL_SCANCODE_LEFT:
//                            tank.x -= speed / 20;
//                            break;
//                        case SDL_SCANCODE_S:
//                        case SDL_SCANCODE_DOWN:
//                            tank.y += speed / 20;
//                            break;
//                        case SDL_SCANCODE_D:
//                        case SDL_SCANCODE_RIGHT:
//                            tank.x += speed / 20;
//                            break;
//                        default:
//                            break;
//                    }
//            }
//        }
//
//        if (tank.x + tank.w > 1000)
//            tank.x = 1000 - tank.w;
//
//        if (tank.x < 0)
//            tank.x = 0;
//
//        if (tank.y + tank.h > 1000)
//            tank.y = 1000 - tank.h;
//
//        if (tank.y < 0)
//            tank.y = 0;
//
//        SDL_RenderClear(renderer);
//        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//        SDL_RenderDrawRect(renderer, &tank);
//
//        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//
//        SDL_RenderPresent(renderer);
//
//        SDL_Delay(1000 / 60);
//    }
//
//    return 0;

















    Application app;
    app.loop();
    return 0;
}