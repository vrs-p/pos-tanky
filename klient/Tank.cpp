//
// Created by vrsp on 28. 12. 2022.
//

#include "Tank.h"

Tank::Tank() {
    this->speed_ = 5.0;
    this->reloadTime_ = 5;

    this->tankIcon_ = new sf::RectangleShape();
    this->tankIcon_->setSize(sf::Vector2f(20, 40));
    this->direction_ = UP;

    this->bullet_ = new Bullet();

}

Tank::~Tank() {
    delete this->tankIcon_;
    delete this->bullet_;
    tankIcon_ = nullptr;
    bullet_ = nullptr;
}

sf::RectangleShape *Tank::getIcon() {
    return this->tankIcon_;
}

double Tank::getSpeed() const {
    return this->speed_;
}

double Tank::getReloadTime() {
    return this->reloadTime_;
}

void Tank::moveUp() {
    this->tankIcon_->move(sf::Vector2f(0.0f, -this->speed_));
    this->tankIcon_->setRotation(0.f);
    this->direction_ = UP;
}

void Tank::moveDown() {
    this->tankIcon_->move(sf::Vector2f(0.0f, this->speed_));
    this->tankIcon_->setRotation(180.f);
    this->direction_ = DOWN;
}

void Tank::moveLeft() {
    this->tankIcon_->move(sf::Vector2f(-this->speed_, 0.0f));
    this->tankIcon_->setRotation(270.f);
    this->direction_ = LEFT;
}

void Tank::moveRight() {
    this->tankIcon_->move(sf::Vector2f(this->speed_, 0.0f));
    this->tankIcon_->setRotation(90.f);
    this->direction_ = RIGHT;
}

void Tank::render(sf::RenderWindow &window) {
    if (this->bullet_->wasFired())
        this->bullet_->render(window);
    window.draw(*this->tankIcon_);
}

void Tank::fire() {
    this->bullet_->shotBullet(this->tankIcon_->getPosition().x, this->tankIcon_->getPosition().y, this->direction_);
}

