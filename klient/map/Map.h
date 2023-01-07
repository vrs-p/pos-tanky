//
// Created by filip on 7. 1. 2023.
//

#ifndef KLIENT_MAP_H
#define KLIENT_MAP_H

#include <SFML/Graphics.hpp>

class Map {
public:
    Map();
    ~Map();

    std::vector<sf::RectangleShape*>* getListOfWalls();
private:
    std::vector<sf::RectangleShape*>* listOfWalls_;
};


#endif //KLIENT_MAP_H
