//
// Created by filip on 7. 1. 2023.
//

#include "Map.h"

Map::Map() {
    this->listOfWalls_ = new std::vector<sf::RectangleShape *>();

    sf::RectangleShape* tmpShape = new sf::RectangleShape();
    tmpShape->setSize(sf::Vector2f(200.0, 2.0));
    tmpShape->setPosition(sf::Vector2f(350.0, 400.0));
    this->listOfWalls_->push_back(tmpShape);

    tmpShape = new sf::RectangleShape();
    tmpShape->setSize(sf::Vector2f(2.0, 200.0));
    tmpShape->setPosition(sf::Vector2f(350.0, 400.0));
    this->listOfWalls_->push_back(tmpShape);
}

Map::~Map() {
    for (int i = 0; i < this->listOfWalls_->size(); ++i) {
        delete this->listOfWalls_->at(i);
    }

    delete this->listOfWalls_;
}

std::vector<sf::RectangleShape *> *Map::getListOfWalls() {
    return this->listOfWalls_;
}
