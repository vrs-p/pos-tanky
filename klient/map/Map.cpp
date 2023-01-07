//
// Created by filip on 7. 1. 2023.
//
#include "Map.h"


/**
 * Constructor for Map class
 */
Map::Map() {
    this->listOfWalls_ = new std::vector<sf::RectangleShape *>();

    sf::RectangleShape* tmpShape = new sf::RectangleShape();

    tmpShape->setSize(sf::Vector2f(100.0, 100.0));
    tmpShape->setPosition(sf::Vector2f(100.0, 100.0));
    this->listOfWalls_->push_back(tmpShape);
    tmpShape = new sf::RectangleShape();

    tmpShape->setSize(sf::Vector2f(150.0, 25.0));
    tmpShape->setPosition(sf::Vector2f(500.0, 180.0));
    this->listOfWalls_->push_back(tmpShape);
    tmpShape = new sf::RectangleShape();

    tmpShape->setSize(sf::Vector2f(10.0, 80.0));
    tmpShape->setPosition(sf::Vector2f(90.0, 450.0));
    this->listOfWalls_->push_back(tmpShape);
    tmpShape = new sf::RectangleShape();

    tmpShape->setSize(sf::Vector2f(30.0, 30.0));
    tmpShape->setPosition(sf::Vector2f(600.0, 600.0));
    this->listOfWalls_->push_back(tmpShape);
    tmpShape = new sf::RectangleShape();

    tmpShape->setSize(sf::Vector2f(20.0, 130.0));
    tmpShape->setPosition(sf::Vector2f(700.0, 650.0));
    this->listOfWalls_->push_back(tmpShape);
    tmpShape = new sf::RectangleShape();

    tmpShape->setSize(sf::Vector2f(45.0, 10.0));
    tmpShape->setPosition(sf::Vector2f(330.0, 330.0));
    this->listOfWalls_->push_back(tmpShape);
    tmpShape = new sf::RectangleShape();

    tmpShape->setSize(sf::Vector2f(45.0, 10.0));
    tmpShape->setPosition(sf::Vector2f(425.0, 330.0));
    this->listOfWalls_->push_back(tmpShape);
    tmpShape = new sf::RectangleShape();

    tmpShape->setSize(sf::Vector2f(10.0, 45.0));
    tmpShape->setPosition(sf::Vector2f(330.0, 330.0));
    this->listOfWalls_->push_back(tmpShape);
    tmpShape = new sf::RectangleShape();

    tmpShape->setSize(sf::Vector2f(10.0, 45.0));
    tmpShape->setPosition(sf::Vector2f(460.0, 330.0));
    this->listOfWalls_->push_back(tmpShape);
    tmpShape = new sf::RectangleShape();

    tmpShape->setSize(sf::Vector2f(10.0, 45.0));
    tmpShape->setPosition(sf::Vector2f(330.0, 425.0));
    this->listOfWalls_->push_back(tmpShape);
    tmpShape = new sf::RectangleShape();

    tmpShape->setSize(sf::Vector2f(10.0, 45.0));
    tmpShape->setPosition(sf::Vector2f(460.0, 425.0));
    this->listOfWalls_->push_back(tmpShape);
    tmpShape = new sf::RectangleShape();

    tmpShape->setSize(sf::Vector2f(45.0, 10.0));
    tmpShape->setPosition(sf::Vector2f(330.0, 460.0));
    this->listOfWalls_->push_back(tmpShape);
    tmpShape = new sf::RectangleShape();

    tmpShape->setSize(sf::Vector2f(45.0, 10.0));
    tmpShape->setPosition(sf::Vector2f(425.0, 460.0));
    this->listOfWalls_->push_back(tmpShape);
}

/**
 * Destructor for Map class
 */
Map::~Map() {
    for (int i = 0; i < this->listOfWalls_->size(); ++i) {
        delete this->listOfWalls_->at(i);
    }
    delete this->listOfWalls_;
    this->listOfWalls_ = nullptr;
}

/**
 * Getter for list of walls
 * @return  list of walls
 */
std::vector<sf::RectangleShape *> *Map::getListOfWalls() {
    return this->listOfWalls_;
}