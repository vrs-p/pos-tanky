//
// Created by filip on 28. 12. 2022.
//
#include "Application.h"
#include "Bullet.h"


//#include "Bullet.h"


Bullet::Bullet() {
    this->speed_ = 15;
    this->fired_ = false;
    this->firedAndSent_ = false;
    this->diameter_ = 4;
    this->bulletIcon_ = new sf::RectangleShape(sf::Vector2f(this->diameter_, this->diameter_));
}

Bullet::~Bullet() {
    delete this->bulletIcon_;
    this->bulletIcon_ = nullptr;
}

void Bullet::shotBullet(float xPosition, float yPosition, DIRECTION direction) {
    switch (direction) {
        case UP:
        case DOWN:
            this->bulletIcon_->setPosition(xPosition - diameter_ / 2, yPosition);
            break;
        case LEFT:
        case RIGHT:
            this->bulletIcon_->setPosition(xPosition, yPosition - diameter_ / 2);
            break;
    }
    this->direction_ = direction;
    this->fired_ = true;
}

void Bullet::render(sf::RenderWindow &window) {
    float xPosition = this->bulletIcon_->getPosition().x;
    float yPosition = this->bulletIcon_->getPosition().y;
    if (xPosition > 0 && xPosition < SCREEN_WIDTH &&
        yPosition > 0 && yPosition < SCREEN_HEIGHT &&
        this->fired_)
    {
        moveBullet();
        window.draw(*this->bulletIcon_);
    } else {
        this->fired_ = false;
        this->firedAndSent_ = false;
    }
}

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

bool Bullet::wasFired() const {
    return this->fired_;
}

bool Bullet::wasFiredAndSent() const {
    return this->firedAndSent_;
}

void Bullet::setWasFiredAndSent() {
    this->firedAndSent_ = true;
}

sf::Vector2f Bullet::getBulletPosition() {
    return this->bulletIcon_->getPosition();
}

sf::Vector2f Bullet::getBulletSize() {
    return this->bulletIcon_->getSize();
}

sf::RectangleShape *Bullet::getBulletIcon() {
    return this->bulletIcon_;
}

