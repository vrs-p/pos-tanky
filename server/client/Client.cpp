//
// Created by filip on 30. 12. 2022.
//

#include "Client.h"

#include <utility>

Client::Client(int id, std::string pName, float xPosition, float yPosition, DIRECTION direction, unsigned short port, sf::IpAddress ipAddress) {
    this->id_ = id;
    this->playerName = std::move(pName);
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

Client::~Client() {
    delete this->connection_;
    this->connection_ = nullptr;

    delete this->position_;
    this->position_ = nullptr;

    delete this->initialPosition_;
    this->initialPosition_ = nullptr;
}

void Client::updatePosition(float xPosition, float yPosition, DIRECTION direction) {
    this->position_->xPosition_ = xPosition;
    this->position_->yPosition_ = yPosition;
    this->position_->direction_ = direction;
}

POSITION *Client::getPosition() {
    return this->position_;
}

CONNECTION *Client::getConnetcion() {
    return this->connection_;
}

int Client::getClientId() {
    return this->id_;
}

bool Client::getFired() {
    return this->fired_;
}

void Client::setFired(bool fired) {
    this->fired_ = fired;
}

void Client::setInitialPosition(float xPosition, float yPosition, DIRECTION direction) {
    this->initialPosition_ = new POSITION();
    this->initialPosition_->xPosition_ = xPosition;
    this->initialPosition_->yPosition_ = yPosition;
    this->initialPosition_->direction_ = direction;
}

void Client::killed() {
    this->killed_ = true;
}

bool Client::wasKilled() {
    return this->killed_;
}

void Client::resetPosition() {
    this->position_->xPosition_ = this->initialPosition_->xPosition_;
    this->position_->yPosition_ = this->initialPosition_->yPosition_;
    this->position_->direction_ = this->initialPosition_->direction_;
    this->killed_ = false;
}

void Client::increaseScore() {
    this->score_++;
}

void Client::setKilledBy(int pId) {
    this->killedBy_ = pId;
}

int Client::getKilledBy() {
    return this->killedBy_;
}

bool Client::getLeft() {
    return this->left_;
}

void Client::setLeft(bool left) {
    this->left_ = left;
}

bool Client::wasScoreSent() {
    return this->scoreWasSent_;
}

bool Client::setScoreWasSent(bool sent) {
    return this->scoreWasSent_ = sent;
}

int Client::getScore() {
    return this->score_;
}

void Client::lockMutex() {
    this->mutex_.lock();
}

void Client::unlockMutex() {
    this->mutex_.unlock();
}

std::string Client::getPlayerName() {
    return this->playerName;
}


