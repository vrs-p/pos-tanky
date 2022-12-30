//
// Created by vrsp on 28. 12. 2022.
//

#include "Tank.h"

Tank::Tank() {
    this->speed_ = 5.0;
    this->reloadTime_ = 3;

    this->tankTexture_ = new sf::Texture();
    this->tankSprite_ = new sf::Sprite();
    this->bullet_ = new Bullet();

    this->tankTexture_->loadFromFile("../../img/tankWithoutBG.png");
    this->tankSprite_->setTexture(*this->tankTexture_);
    this->tankSprite_->setScale(sf::Vector2f(0.05,0.05));
    this->direction_ = UP;


    this->lastFire_ = std::chrono::system_clock::now();
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
    if (this->direction_ != UP) {
        float xSize = this->tankSprite_->getTexture()->getSize().x * this->tankSprite_->getScale().x;
        float ySize = this->tankSprite_->getTexture()->getSize().y * this->tankSprite_->getScale().y;
        switch (this->direction_) {
            case DOWN:
                this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x - xSize,
                        this->tankSprite_->getPosition().y - ySize));
                break;
            case LEFT:
                this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x + (ySize - xSize),
                        this->tankSprite_->getPosition().y - ySize));
                break;
            case RIGHT:
                this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x - ySize,
                        this->tankSprite_->getPosition().y - (ySize - xSize)));
                break;
        }
    }
    this->direction_ = UP;
    this->tankSprite_->setRotation(0);
    this->tankSprite_->move(sf::Vector2f(0, -this->speed_));
}

void Tank::moveDown() {
    if (this->direction_ != DOWN) {
        float xSize = this->tankSprite_->getTexture()->getSize().x * this->tankSprite_->getScale().x;
        float ySize = this->tankSprite_->getTexture()->getSize().y * this->tankSprite_->getScale().y;
        switch (this->direction_) {
            case UP:
                this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x + xSize,
                                                            this->tankSprite_->getPosition().y + ySize));
                break;
            case LEFT:
                this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x + ySize,
                                                            this->tankSprite_->getPosition().y + (ySize - xSize)));
                break;
            case RIGHT:
                this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x - (ySize - xSize),
                                                            this->tankSprite_->getPosition().y + ySize));
                break;
        }
    }
    this->direction_ = DOWN;
    this->tankSprite_->setRotation(180);
    this->tankSprite_->move(sf::Vector2f(0, this->speed_));
}

void Tank::moveLeft() {
    if (this->direction_ != LEFT) {
        float xSize = this->tankSprite_->getTexture()->getSize().x * this->tankSprite_->getScale().x;
        float ySize = this->tankSprite_->getTexture()->getSize().y * this->tankSprite_->getScale().y;
        switch (this->direction_) {
            case UP:
                this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x + xSize - ySize,
                                                            this->tankSprite_->getPosition().y + ySize));
                break;
            case DOWN:
                this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x - ySize,
                                                            this->tankSprite_->getPosition().y - ySize + xSize));
                break;
            case RIGHT:
                this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x - ySize,
                                                            this->tankSprite_->getPosition().y + xSize));
                break;
        }
    }
    this->direction_ = LEFT;
    this->tankSprite_->setRotation(270);
    this->tankSprite_->move(sf::Vector2f(-this->speed_, 0));
}

void Tank::moveRight() {
    if (this->direction_ != RIGHT) {
        float xSize = this->tankSprite_->getTexture()->getSize().x * this->tankSprite_->getScale().x;
        float ySize = this->tankSprite_->getTexture()->getSize().y * this->tankSprite_->getScale().y;
        switch (this->direction_) {
            case UP:
                this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x + ySize,
                                                            this->tankSprite_->getPosition().y + ySize - xSize));
                break;
            case DOWN:
                this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x - xSize + ySize,
                                                            this->tankSprite_->getPosition().y - ySize));
                break;
            case LEFT:
                this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x + ySize,
                                                            this->tankSprite_->getPosition().y - xSize));
                break;
        }
    }
    this->direction_ = RIGHT;
    this->tankSprite_->setRotation(90);
    this->tankSprite_->move(sf::Vector2f(this->speed_, 0));
}

void Tank::render(sf::RenderWindow &window) {
    if (this->bullet_->wasFired())
        this->bullet_->render(window);
    window.draw(*this->tankSprite_);
}

void Tank::fire() {
    std::chrono::duration<double> duration = std::chrono::system_clock::now() - this->lastFire_;
    if (duration.count() > this->reloadTime_) {
        float xSize = this->tankSprite_->getTexture()->getSize().x * this->tankSprite_->getScale().x;
        switch (this->direction_) {
            case UP:
                this->bullet_->shotBullet(this->tankSprite_->getPosition().x + xSize / 2,
                                          this->tankSprite_->getPosition().y, this->direction_);
                break;
            case DOWN:
                this->bullet_->shotBullet(this->tankSprite_->getPosition().x - xSize / 2,
                                          this->tankSprite_->getPosition().y, this->direction_);
                break;
            case LEFT:
                this->bullet_->shotBullet(this->tankSprite_->getPosition().x,
                                          this->tankSprite_->getPosition().y - xSize / 2, this->direction_);
                break;
            case RIGHT:
                this->bullet_->shotBullet(this->tankSprite_->getPosition().x,
                                          this->tankSprite_->getPosition().y + xSize / 2, this->direction_);
                break;
        }
        this->lastFire_ = std::chrono::system_clock::now();
    }
}

int Tank::getDirection() {
    return static_cast<int>(this->direction_);
}

