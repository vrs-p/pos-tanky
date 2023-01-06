//
// Created by filip on 30. 12. 2022.
//

#include "Client.h"

#include <utility>

/**
 * Constructor for Client class.
 * @param id ID of player.
 * @param pName Name of player.
 * @param xPosition X axis position.
 * @param yPosition Y axis position.
 * @param direction Direction of player.
 * @param port Port on which player is listening.
 * @param ipAddress IP address of player
 */
Client::Client(int id, std::string pName, float xPosition, float yPosition, DIRECTION direction, unsigned short port, sf::IpAddress ipAddress) {
    this->id_ = id;
    this->playerName_ = std::move(pName);
    this->position_ = new POSITION;
    this->connection_ = new CONNECTION;
    this->position_->xPosition_ = xPosition;
    this->position_->yPosition_ = yPosition;
    this->position_->direction_ = direction;
    this->connection_->port_ = port;
    this->connection_->ipAddress_ = ipAddress;
    this->fired_ = false;
    this->score_ = 0;
    this->left_ = false;
    this->scoreWasSent_ = false;
}

/**
 * Destructor for Client class.
 */
Client::~Client() {
    delete this->connection_;
    this->connection_ = nullptr;

    delete this->position_;
    this->position_ = nullptr;

    delete this->initialPosition_;
    this->initialPosition_ = nullptr;
}

/**
 * By calling this method you are able to update actual position of player.
 * @param xPosition X axis position.
 * @param yPosition Y axis position.
 * @param direction Direction of player.
 */
void Client::updatePosition(float xPosition, float yPosition, DIRECTION direction) {
    this->position_->xPosition_ = xPosition;
    this->position_->yPosition_ = yPosition;
    this->position_->direction_ = direction;
}

/**
 * By calling this method you'll be able to access to struct POSITION.
 * You'll be able to access to actual xPosition, yPosition and direction of player.
 * @return POSITION*
 */
POSITION *Client::getPosition() {
    return this->position_;
}

/**
 * By calling this method you'll be able to access to struct CONNECTION.
 * You'll be able to access to player's IP address and port.
 * @return CONNECTION*
 */
CONNECTION *Client::getConnection() {
    return this->connection_;
}

/**
 * Getter for player ID.
 * @return ID of player
 */
int Client::getClientId() {
    return this->id_;
}

/**
 * Getter for information if player fired bullet.
 * @return if player fired.
 */
bool Client::getFired() {
    return this->fired_;
}

/**
 * Setter for information if player fired bullet.
 * @param fired bool if player fired
 */
void Client::setFired(bool fired) {
    this->fired_ = fired;
}

/**
 * Setter for initial position of players.
 * Used for respawn of players.
 * @param xPosition X axis position.
 * @param yPosition Y axis position.
 * @param direction Direction of player.
 */
void Client::setInitialPosition(float xPosition, float yPosition, DIRECTION direction) {
    this->initialPosition_ = new POSITION();
    this->initialPosition_->xPosition_ = xPosition;
    this->initialPosition_->yPosition_ = yPosition;
    this->initialPosition_->direction_ = direction;
}

/**
 * Setter for information that player was killed.
 */
void Client::killed() {
    this->killed_ = true;
}

/**
 * Getter for information if player is dead
 * @return if player is dead.
 */
bool Client::wasKilled() {
    return this->killed_;
}

/**
 * By calling this method you'll put player to initial position.
 * Used for respawn.
 */
void Client::resetPosition() {
    this->position_->xPosition_ = this->initialPosition_->xPosition_;
    this->position_->yPosition_ = this->initialPosition_->yPosition_;
    this->position_->direction_ = this->initialPosition_->direction_;
    this->killed_ = false;
}

/**
 * By calling this method you'll increase score of player by 1.
 */
void Client::increaseScore() {
    this->score_++;
}

/**
 * Setter for information of by which player was this player killed.
 * @param pId ID of killer
 */
void Client::setKilledBy(int pId) {
    this->killedBy_ = pId;
}

/**
 * Getter for information of by which player was this player killed.
 * @return ID of killer
 */
int Client::getKilledBy() {
    return this->killedBy_;
}

/**
 * Getter for information if this player left game.
 * @return if player left game
 */
bool Client::getLeft() {
    return this->left_;
}

/**
 * Setter for information that player left.
 * @param left if player left game
 */
void Client::setLeft(bool left) {
    this->left_ = left;
}

/**
 * Getter for information if score was sent to player.
 * Used when player left game.
 * @return
 */
bool Client::wasScoreSent() {
    return this->scoreWasSent_;
}

/**
 * Setter for information that score was sent
 * @param sent
 */
void Client::setScoreWasSent(bool sent) {
    this->scoreWasSent_ = sent;
}

/**
 * Getter for score of player.
 * @return Score of player
 */
int Client::getScore() {
    return this->score_;
}

/**
 * By calling this method you'll lock mutex.
 */
void Client::lockMutex() {
    this->mutex_.lock();
}

/**
 * By calling this method you'll unlock mutex.
 */
void Client::unlockMutex() {
    this->mutex_.unlock();
}

/**
 * Getter for name of player
 * @return name of player
 */
std::string Client::getPlayerName() {
    return this->playerName_;
}


