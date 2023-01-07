#pragma once
//
// Created by filip on 28. 12. 2022.
//
#include <SFML/Graphics.hpp>

enum DIRECTION {UP,
    DOWN,
    LEFT,
    RIGHT};

class Bullet{
public:
    Bullet();
    ~Bullet();

    void render(sf::RenderWindow& renderer, std::vector<sf::RectangleShape *>* listOfWalls);
    void shotBullet(float xPosition, float yPosition, DIRECTION direction);
    void setFired(bool fired);
    void setWasFiredAndSent();
    void resetWasFiredAndSent();
    sf::Vector2f getBulletPosition();
    sf::Vector2f getBulletSize();
    bool wasFired() const;
    bool wasFiredAndSent() const;
    bool checkBorders(std::vector<sf::RectangleShape *>* listOfWalls);

private:
    float speed_;
    int diameter_;
    sf::RectangleShape* bulletIcon_;
    bool fired_;
    bool firedAndSent_;
    DIRECTION direction_;

    void moveBullet();
};