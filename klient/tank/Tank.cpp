//
// Created by vrsp on 28. 12. 2022.
//
#include "Tank.h"

Tank::Tank() {
    this->left_ = false;

    this->tankTexture_ = new sf::Texture();
    this->tankSprite_ = new sf::Sprite();
    this->tankTexture_->loadFromFile("../../img/tankWithoutBG.png");
    this->tankSprite_->setTexture(*this->tankTexture_);
    this->tankSprite_->setScale(sf::Vector2f(0.05,0.05));

    this->bullet_ = new Bullet();
    this->speed_ = 5.0;
    this->reloadTime_ = 3;
    this->direction_ = UP;

    this->lastFire_ = std::chrono::system_clock::now();
}

Tank::~Tank() {
    delete this->tankSprite_;
    tankSprite_ = nullptr;

    delete this->tankTexture_;
    tankTexture_ = nullptr;

    delete this->bullet_;
    bullet_ = nullptr;
}

void Tank::moveUp() {
    this->rotate(UP);
    this->tankSprite_->move(sf::Vector2f(0, -this->speed_));
}

void Tank::moveDown() {
    this->rotate(DOWN);
    this->tankSprite_->move(sf::Vector2f(0, this->speed_));
}

void Tank::moveLeft() {
    this->rotate(LEFT);
    this->tankSprite_->move(sf::Vector2f(-this->speed_, 0));
}

void Tank::moveRight() {
    this->rotate(RIGHT);
    this->tankSprite_->move(sf::Vector2f(this->speed_, 0));
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

void Tank::rotate(DIRECTION dir) {
    float xSize = this->tankSprite_->getTexture()->getSize().x * this->tankSprite_->getScale().x;
    float ySize = this->tankSprite_->getTexture()->getSize().y * this->tankSprite_->getScale().y;

    switch (dir) {
        case UP:
            switch (this->direction_) {
                case DOWN:
                    this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x - xSize,
                                                                this->tankSprite_->getPosition().y - ySize * 4 / 3));
                    break;

                case LEFT:
                    this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x + ySize - xSize,
                                                                this->tankSprite_->getPosition().y - ySize));
                    break;

                case RIGHT:
                    this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x - ySize,
                                                                this->tankSprite_->getPosition().y - ySize + xSize));
                    break;

                default:
                    break;
            }

            this->tankSprite_->setRotation(0);
            break;

        case DOWN:
            switch (this->direction_) {
                case UP:
                    this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x + xSize,
                                                                this->tankSprite_->getPosition().y + ySize * 4 / 3));
                    break;

                case LEFT:
                    this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x + ySize,
                                                                this->tankSprite_->getPosition().y + ySize - xSize));
                    break;

                case RIGHT:
                    this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x - ySize + xSize,
                                                                this->tankSprite_->getPosition().y + ySize));
                    break;
            }

            this->tankSprite_->setRotation(180);
            break;

        case LEFT:
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
                    this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x - ySize * 4 / 3,
                                                                this->tankSprite_->getPosition().y + xSize));
                    break;
            }

            this->tankSprite_->setRotation(270);
            break;

        case RIGHT:
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
                    this->tankSprite_->setPosition(sf::Vector2f(this->tankSprite_->getPosition().x + ySize * 4 / 3,
                                                                this->tankSprite_->getPosition().y - xSize));
                    break;
            }

            this->tankSprite_->setRotation(90);
            break;
    }

    this->direction_ = dir;
}

void Tank::render(sf::RenderWindow &window) {
    if (this->bullet_->wasFired()) {
        this->bullet_->render(window);
    }

    window.draw(*this->tankSprite_);
}

void Tank::lockMutex() {
    this->mutex_.lock();
}

void Tank::unlockMutex() {
    this->mutex_.unlock();
}

void Tank::setDirection(DIRECTION dir) {
    this->direction_ = dir;
}

void Tank::setPlayerId(int pId) {
    this->playerId_ = pId;
}

void Tank::setPlayerName(std::string name) {
    this->playerName_ = name;
}

void Tank::setLeft(bool left) {
    this->left_ = left;
}

void Tank::setScore(int score) {
    this->score_ = score;
}

sf::Sprite *Tank::getSprite() {
    return this->tankSprite_;
}

int Tank::getDirection() {
    return static_cast<int>(this->direction_);
}

int Tank::getPlayerId() const {
    return this->playerId_;
}

std::string Tank::getPlayerName() {
    return this->playerName_;
}

Bullet *Tank::getBullet() {
    return this->bullet_;
}

bool Tank::getLeft() {
    return this->left_;
}

int Tank::getScore() {
    return this->score_;
}