//
// Created by vrsp on 5. 1. 2023.
//
#include <SFML/Graphics.hpp>
#include "../tank/Tank.h"

#ifndef KLIENT_SHOWSCORE_H
#define KLIENT_SHOWSCORE_H


class ShowScore {
public:
    ShowScore(int playerScore, std::vector<Tank*>* otherTanks);
    ~ShowScore();

    void render();

private:
    bool showScore_;

    sf::RenderWindow* window_;
    sf::Font font_;
    sf::Text textGame_;
    sf::Text textEndGame_;
    sf::Text textYourScore_;
    sf::Text textOthersScore_;
    std::string stringOthersScore_;

    void initializeWindow();
};


#endif //KLIENT_SHOWSCORE_H