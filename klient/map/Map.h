//
// Created by filip on 7. 1. 2023.
//
#include <SFML/Graphics.hpp>

#ifndef KLIENT_MAP_H
#define KLIENT_MAP_H


class Map {
public:
    Map();
    ~Map();

    std::vector<sf::RectangleShape*>* getListOfWalls();

private:
    std::vector<sf::RectangleShape*>* listOfWalls_;
};


#endif //KLIENT_MAP_H
