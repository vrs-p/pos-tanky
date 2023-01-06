#pragma once
//
// Created by vrsp on 28. 12. 2022.
//
#include <iostream>
#include <list>
#include <thread>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <condition_variable>
#include <mutex>
#include "../tank/Tank.h"

#ifndef KLIENT_APPLICATION_H
#define KLIENT_APPLICATION_H

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;


enum TYPES_MESSAGES {
    STATUS,
    KILLED,
    END,
    PLAYER_QUIT
};


class Application {
public:
    Application();
    ~Application();

    void run(sf::IpAddress ipAddress, int port, std::string playerName);
    int getPlayerScore();
    std::vector<Tank*>* getOthersTanks();

private:
    sf::IpAddress ipAddress_;
    int port_;
    sf::UdpSocket socket_;
    sf::Packet packetSend_;
    unsigned short id_;

    int numberOfPlayers_;
    bool isRunning_;
    bool sendDataBool_;
    bool playerWasKilled_;
    int idOfKilledPlayer_;

    std::mutex* mutex_;
    std::condition_variable* sendDataCond_;
    sf::RenderWindow* window_;
    sf::Font font_;
    sf::Text nameOfPlayer_;
    Tank* clientTank_;
    std::vector<Tank*>* otherTanks_;

    void communicationWithServer();
    void connectToServer();

    void waitForGameSettings();
    void render();
    void initializeWindow();
    void readClientInput();
    void checkBorders();
    void checkBulletCollision();

    void draw();

    void receiveData();
    void sendData();

    void updatePositionsOfTanks();
};


#endif //KLIENT_APPLICATION_H