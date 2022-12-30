//
// Created by vrsp on 30. 12. 2022.
//

#ifndef SERVER_TANK_H
#define SERVER_TANK_H
#include <SFML/Graphics.hpp>
#include <chrono>
#include "Bullet.h"


class Tank {
public:
    Tank();
    ~Tank();

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void fire();

    void rotate(DIRECTION dir);

    void render(sf::RenderWindow& renderer);

    sf::Sprite * getSprite();

    int getDirection();
    void setDirection(DIRECTION dir);
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

    std::chrono::time_point<std::chrono::system_clock> lastFire_;
};


#endif //SERVER_TANK_H
