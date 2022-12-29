//
// Created by vrsp on 28. 12. 2022.
//

#include "Tank.h"

Tank::Tank() {
    this->speed_ = 5.0;
    this->reloadTime_ = 5;

    this->tankTexture_ = new sf::Texture();
    this->tankTexture_->loadFromFile("../img/tankWithoutBG.png");
    this->tankSprite_ = new sf::Sprite();
    this->tankSprite_->setTexture(*this->tankTexture_);
    this->tankSprite_->setScale(sf::Vector2f(0.05,0.05));
    this->direction_ = UP;

    this->bullet_ = new Bullet();

}

Tank::~Tank() {
    delete this->tankSprite_;
    delete this->tankTexture_;
    delete this->bullet_;

    tankSprite_ = nullptr;
    tankTexture_ = nullptr;
    bullet_ = nullptr;
}

sf::Sprite *Tank::getSprite() {
    return this->tankSprite_;
}

double Tank::getSpeed() const {
    return this->speed_;
}

double Tank::getReloadTime() {
    return this->reloadTime_;
}

void Tank::moveUp() {
    this->tankSprite_->move(sf::Vector2f(0, -this->speed_));
    this->tankSprite_->setRotation(0);
    this->direction_ = UP;
}

void Tank::moveDown() {
    this->tankSprite_->move(sf::Vector2f(0, this->speed_));
    this->tankSprite_->setRotation(180);
    this->direction_ = DOWN;
}

void Tank::moveLeft() {
    this->tankSprite_->move(sf::Vector2f(-this->speed_, 0));
    this->tankSprite_->setRotation(270);
    this->direction_ = LEFT;
}

void Tank::moveRight() {
    this->tankSprite_->move(sf::Vector2f(this->speed_, 0));
    this->tankSprite_->setRotation(90);
    this->direction_ = RIGHT;
}

void Tank::render(sf::RenderWindow &window) {
    if (this->bullet_->wasFired())
        this->bullet_->render(window);
    window.draw(*this->tankSprite_);
}

void Tank::fire() {
    this->bullet_->shotBullet(this->tankSprite_->getPosition().x + this->tankSprite_->getTexture()->getSize().x * this->tankSprite_->getScale().x / 2, this->tankSprite_->getPosition().y, this->direction_);
}

