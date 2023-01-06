//
// Created by filip on 28. 12. 2022.
//
#include "Bullet.h"


Bullet::Bullet() {
    this->speed_ = 15;
    this->diameter_ = 4;
    this->bulletIcon_ = new sf::RectangleShape(sf::Vector2f(this->diameter_, this->diameter_));
    this->fired_ = false;
    this->firedAndSent_ = false;
}

Bullet::~Bullet() {
    delete this->bulletIcon_;
    this->bulletIcon_ = nullptr;
}

void Bullet::render(sf::RenderWindow &window) {
    float xPosition = this->bulletIcon_->getPosition().x;
    float yPosition = this->bulletIcon_->getPosition().y;

    if (xPosition > 0 && xPosition < 800 && yPosition > 0 && yPosition < 800 && this->fired_) {
        moveBullet();
        window.draw(*this->bulletIcon_);
    } else {
        this->fired_ = false;
        this->firedAndSent_ = false;
    }
}

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

void Bullet::setFired(bool fired) {
    this->fired_ = fired;
}

void Bullet::setWasFiredAndSent() {
    this->firedAndSent_ = true;
}

void Bullet::resetWasFiredAndSent() {
    this->firedAndSent_ = false;
}

sf::Vector2f Bullet::getBulletPosition() {
    return this->bulletIcon_->getPosition();
}

sf::Vector2f Bullet::getBulletSize() {
    return this->bulletIcon_->getSize();
}

bool Bullet::wasFired() const {
    return this->fired_;
}

bool Bullet::wasFiredAndSent() const {
    return this->firedAndSent_;
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