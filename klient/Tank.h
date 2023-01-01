//
// Created by vrsp on 28. 12. 2022.
//

#ifndef KLIENT_TANK_H
#define KLIENT_TANK_H
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

    int getPlayerId() const;
    void setPlayerId(int pId);

    Bullet* getBullet();

private:
    int playerId;

    sf::Texture* tankTexture_;
    sf::Sprite* tankSprite_;

    sf::RectangleShape* tankIcon_;
    Bullet* bullet_;
    float speed_;
    double reloadTime_;
    DIRECTION direction_;

    std::chrono::time_point<std::chrono::system_clock> lastFire_;
};


#endif //KLIENT_TANK_H
