//
// Created by vrsp on 28. 12. 2022.
//

#ifndef KLIENT_TANK_H
#define KLIENT_TANK_H
#include "SDL.h"
#include "Bullet.h"
//#include <SDL_image.h>


class Tank {
public:
    Tank();
    ~Tank();

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void fire();

    void render(SDL_Renderer& renderer);

    SDL_Rect * getIcon();
    double getSpeed() const;
    double getReloadTime();

private:
    SDL_Rect *tankIcon_;
    Bullet* bullet_;
    double speed_;
    double reloadTime_;
    DIRECTION direction_;
};


#endif //KLIENT_TANK_H
