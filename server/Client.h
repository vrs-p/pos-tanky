//
// Created by filip on 30. 12. 2022.
//
#include <SFML/Network.hpp>
#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H
enum DIRECTION {UP,
    DOWN,
    LEFT,
    RIGHT};

struct CONNECTION {
    unsigned short port_;
    sf::IpAddress ipAddress_;
};

struct POSITION {
    float xPosition_;
    float yPosition_;
    DIRECTION direction_;
};

class Client {
public:
    Client(int id, float xPosition, float yPosition, DIRECTION direction, unsigned short port, sf::IpAddress ipAddress);

    void updatePosition(float xPosition, float yPosition, DIRECTION direction);

    POSITION* getPosition();
    CONNECTION* getConnetcion();
    int getClientId();

private:
    int id_;
    CONNECTION* connection_;
    POSITION* position_;

};


#endif //SERVER_CLIENT_H
