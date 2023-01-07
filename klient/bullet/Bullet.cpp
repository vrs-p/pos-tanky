//
// Created by filip on 28. 12. 2022.
//
#include <iostream>
#include "Bullet.h"

/**
 * Constructor for Bullet class
 */
Bullet::Bullet() {
    this->speed_ = 15;
    this->diameter_ = 4;
    this->bulletIcon_ = new sf::RectangleShape(sf::Vector2f(this->diameter_, this->diameter_));
    this->fired_ = false;
    this->firedAndSent_ = false;
}

/**
 * Destructor for bullet class
 */
Bullet::~Bullet() {
    delete this->bulletIcon_;
    this->bulletIcon_ = nullptr;
}

/**
 * By calling this function you'll render bullet in window
 * @param window window in which we want to render bullet
 */
void Bullet::render(sf::RenderWindow &window, std::vector<sf::RectangleShape *>* listOfWalls) {
    float xPosition = this->bulletIcon_->getPosition().x;
    float yPosition = this->bulletIcon_->getPosition().y;

    if (this->checkBorders(listOfWalls) && this->fired_) {
        moveBullet();
        window.draw(*this->bulletIcon_);
    } else {
        this->fired_ = false;
        this->firedAndSent_ = false;
    }
}

/**
 * By calling this function you'll initialize bullets position and direction
 * @param xPosition X axis position of bullet
 * @param yPosition Y axis position of bullet
 * @param direction direction of bullet
 */
void Bullet::shotBullet(float xPosition, float yPosition, DIRECTION direction) {
    switch (direction) {
        case UP:
        case DOWN:
            this->bulletIcon_->setPosition(xPosition - this->diameter_ / 2, yPosition);
            break;

        case LEFT:
        case RIGHT:
            this->bulletIcon_->setPosition(xPosition, yPosition - this->diameter_ / 2);
            break;
    }

    this->fired_ = true;
    this->direction_ = direction;
}

/**
 * Setter for information if bullet was fired
 * @param fired if bullet was fired
 */
void Bullet::setFired(bool fired) {
    this->fired_ = fired;
}

/**
 * Setter for information that data about fired bullet were sent
 */
void Bullet::setWasFiredAndSent() {
    this->firedAndSent_ = true;
}

/**
 * By calling this function you'll set firedAndSent_ to false
 */
void Bullet::resetWasFiredAndSent() {
    this->firedAndSent_ = false;
}

/**
 * By calling this function you'll get position of bullet
 * @return position of bullet
 */
sf::Vector2f Bullet::getBulletPosition() {
    return this->bulletIcon_->getPosition();
}

/**
 * By calling this function you'll get size of bullet
 * @return size of bullet
 */
sf::Vector2f Bullet::getBulletSize() {
    return this->bulletIcon_->getSize();
}

/**
 * Getter for information if bullet was fired
 * @return if bullet was fired
 */
bool Bullet::wasFired() const {
    return this->fired_;
}

/**
 * Getter for information of data about fired bullet were sent
 * @return
 */
bool Bullet::wasFiredAndSent() const {
    return this->firedAndSent_;
}

/**
 * By calling this function you'll move bullet in its direction
 */
void Bullet::moveBullet() {
    if (this->direction_ == UP){
        this->bulletIcon_->move(sf::Vector2f(0.0f, -this->speed_));
    } else if (this->direction_ == DOWN) {
        this->bulletIcon_->move(sf::Vector2f(0.0f, this->speed_));
    } else if (this->direction_ == LEFT) {
        this->bulletIcon_->move(sf::Vector2f(-this->speed_, 0.0f));
    } else {
        this->bulletIcon_->move(sf::Vector2f(this->speed_, 0.0f));
    }
}

/**
 * By calling this function it'll check if bullet is not outside of borders
 * It also checks if bullet is touching the wall
 * @param listOfWalls
 * @return
 */
bool Bullet::checkBorders(std::vector<sf::RectangleShape *>* listOfWalls) {
    bool canContinue = true;
    float xPosition = this->bulletIcon_->getPosition().x;
    float yPosition = this->bulletIcon_->getPosition().y;

    for (sf::RectangleShape* wall: *listOfWalls) {
        if (wall->getGlobalBounds().intersects(this->bulletIcon_->getGlobalBounds())) {
            canContinue = false;
        }
    }

    if (xPosition < 0 || xPosition > 800 || yPosition < 0 || yPosition > 800) {
        canContinue = false;
    }

    return canContinue;
}