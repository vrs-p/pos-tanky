//
// Created by filip on 5. 1. 2023.
//
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "elements/Textbox.h"
#include "elements/Button.h"

#ifndef KLIENT_MENU_H
#define KLIENT_MENU_H


class Menu {
public:
    Menu();
    ~Menu();

    void initializeWindow();
    void render();

    std::string getName();
    sf::IpAddress getIpAddress();
    int getPort();
    bool getAppClosed();
private:
    bool gameStarted_;
    bool appClosed_;

    sf::Font font_;
    sf::Text textMainMenu_;
    sf::Text name_;
    sf::Text ipAddressText_;
    sf::Text portText_;

    sf::RenderWindow* window_;
    sf::IpAddress ipAddress_;
    unsigned short int port_;

    std::vector<Textbox*>* textboxes_;
    Button* button_;
};


#endif //KLIENT_MENU_H
