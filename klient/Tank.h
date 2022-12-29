//
// Created by vrsp on 28. 12. 2022.
//

#ifndef KLIENT_TANK_H
#define KLIENT_TANK_H
#include <SFML/Graphics.hpp>
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

    void render(sf::RenderWindow& renderer);

    sf::Sprite * getSprite();

    sf::RectangleShape * getIcon();
    double getSpeed() const;
    double getReloadTime();

private:
    sf::Texture* tankTexture_;
    sf::Sprite* tankSprite_;

    sf::RectangleShape* tankIcon_;
    Bullet* bullet_;
    float speed_;
    double reloadTime_;
    DIRECTION direction_;
};


#endif //KLIENT_TANK_H
