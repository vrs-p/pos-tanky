#pragma once
//
// Created by vrsp on 28. 12. 2022.
//
#include <SFML/Graphics.hpp>
#include <chrono>
#include <mutex>
#include "../bullet/Bullet.h"

#ifndef KLIENT_TANK_H
#define KLIENT_TANK_H


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

    void render(sf::RenderWindow& renderer, std::vector<sf::RectangleShape *>* listOfWalls);

    void lockMutex();
    void unlockMutex();

    void setDirection(DIRECTION dir);
    void setPlayerId(int pId);
    void setPlayerName(std::string name);
    void setLeft(bool left);
    void setScore(int score);

    sf::Sprite * getSprite();
    int getDirection();
    int getPlayerId() const;
    std::string getPlayerName();
    Bullet* getBullet();
    bool getLeft();
    int getScore();

private:
    int playerId_;
    std::string playerName_;
    bool left_;
    int score_;

    sf::Texture* tankTexture_;
    sf::Sprite* tankSprite_;
    std::mutex mutex_;

    Bullet* bullet_;
    float speed_;
    double reloadTime_;
    DIRECTION direction_;

    std::chrono::time_point<std::chrono::system_clock> lastFire_;
};


#endif //KLIENT_TANK_H