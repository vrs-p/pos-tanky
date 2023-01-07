//
// Created by vrsp on 28. 12. 2022.
//
#include "Tank.h"

/**
 * Constructor for Tank class.
 */
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

/**
 * Destructor for Tank class.
 */
Tank::~Tank() {
    delete this->tankSprite_;
    tankSprite_ = nullptr;

    delete this->tankTexture_;
    tankTexture_ = nullptr;

    delete this->bullet_;
    bullet_ = nullptr;
}

/**
 * By calling this function you'll move tank UP and change its direction.
 */
void Tank::moveUp() {
    this->rotate(UP);
    this->tankSprite_->move(sf::Vector2f(0, -this->speed_));
}

/**
 * By calling this function you'll move tank DOWN and change its direction.
 */
void Tank::moveDown() {
    this->rotate(DOWN);
    this->tankSprite_->move(sf::Vector2f(0, this->speed_));
}

/**
 * By calling this function you'll move tank LEFT and change its direction.
 */
void Tank::moveLeft() {
    this->rotate(LEFT);
    this->tankSprite_->move(sf::Vector2f(-this->speed_, 0));
}

/**
 * By calling this function you'll move tank RIGHT and change its direction.
 */
void Tank::moveRight() {
    this->rotate(RIGHT);
    this->tankSprite_->move(sf::Vector2f(this->speed_, 0));
}

/**
 * By calling this function you'll fire from tank.
 */
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

/**
 * By calling this function you are able to change direction of tank
 * @param dir new direction of tank
 */
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

/**
 * By calling this function you'll render icon of tank.
 * And if tank fired a bullet you'll call render function of bullet too.
 * @param window window on which we want to render icons
 */
void Tank::render(sf::RenderWindow &window) {
    if (this->bullet_->wasFired()) {
        this->bullet_->render(window);
    }

    window.draw(*this->tankSprite_);
}

/**
 * By calling this function you'll lock mutex
 */
void Tank::lockMutex() {
    this->mutex_.lock();
}

/**
 * By calling this function you'll unlock mutex
 */
void Tank::unlockMutex() {
    this->mutex_.unlock();
}

/**
 * Setter for direction of tank
 * @param dir new direction
 */
void Tank::setDirection(DIRECTION dir) {
    this->direction_ = dir;
}

/**
 * Setter for player ID
 * @param pId
 */
void Tank::setPlayerId(int pId) {
    this->playerId_ = pId;
}

/**
 * Setter for player name
 * @param name
 */
void Tank::setPlayerName(std::string name) {
    this->playerName_ = name;
}

/**
 * By calling this function you are able to set information if player left the game
 */
void Tank::setLeft(bool left) {
    this->left_ = left;
}

/**
 * Setter for score of player.
 * @param score Score of player
 */
void Tank::setScore(int score) {
    this->score_ = score;
}

/**
 * Getter for sprite.
 * It's icon of tank where is stored position and size of tank.
 * @return
 */
sf::Sprite *Tank::getSprite() {
    return this->tankSprite_;
}

/**
 * Getter for direction of tank
 * @return Direction of tank
 */
int Tank::getDirection() {
    return static_cast<int>(this->direction_);
}

/**
 * Getter for player ID
 * @return Player ID
 */
int Tank::getPlayerId() const {
    return this->playerId_;
}

/**
 * Getter for name of player
 * @return Name of player
 */
std::string Tank::getPlayerName() {
    return this->playerName_;
}

/**
 * Getter for bullet of tank
 * @return Instance of Bullet class
 */
Bullet *Tank::getBullet() {
    return this->bullet_;
}

/**
 * Getter for information if player left the game
 * @return if player left the game
 */
bool Tank::getLeft() {
    return this->left_;
}

/**
 * Getter for score of player.
 * @return score of player
 */
int Tank::getScore() {
    return this->score_;
}