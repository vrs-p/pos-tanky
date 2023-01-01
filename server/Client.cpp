//
// Created by filip on 30. 12. 2022.
//

#include "Client.h"

Client::Client(int id, float xPosition, float yPosition, DIRECTION direction, unsigned short port, sf::IpAddress ipAddress) {
    this->id_ = id;
    this->position_ = new POSITION;
    this->connection_ = new CONNECTION;
    this->position_->xPosition_ = xPosition;
    this->position_->yPosition_ = yPosition;
    this->position_->direction_ = direction;
    this->connection_->port_ = port;
    this->connection_->ipAddress_ = ipAddress;
    this->fired_ = false;
}

Client::~Client() {
    delete this->connection_;
    this->connection_ = nullptr;

    delete this->position_;
    this->position_ = nullptr;

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


