//
// Created by filip on 28. 12. 2022.
//
#include "Bullet.h"

Bullet::Bullet() {
    this->speed_ = 0.5;
    this->fired_ = false;
    this->bulletIcon_ = new sf::CircleShape(5.0f);
}

Bullet::~Bullet() {
    delete this->bulletIcon_;
    this->bulletIcon_ = nullptr;
}

void Bullet::shotBullet(int xPosition, int yPosition, DIRECTION direction) {
    this->bulletIcon_->setPosition(xPosition, yPosition);
    this->direction_ = direction;
    this->fired_ = true;
}

void Bullet::render(sf::RenderWindow &window) {
    float xPosition = this->bulletIcon_->getPosition().x;
    float yPosition = this->bulletIcon_->getPosition().y;
    if (xPosition > 0 && xPosition < 800 &&
        yPosition > 0 && yPosition < 800 &&
        this->fired_)
    {
        moveBullet();
        window.draw(*this->bulletIcon_);
    } else {
        this->fired_ = false;
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
