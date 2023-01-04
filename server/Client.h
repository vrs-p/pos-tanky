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
    ~Client();

    void updatePosition(float xPosition, float yPosition, DIRECTION direction);

    POSITION* getPosition();
    CONNECTION* getConnetcion();
    int getClientId();

    bool getFired();
    void setFired(bool fired);
    void setInitialPosition(float xPosition, float yPosition, DIRECTION direction);
    void resetPosition();
    void killed();
    bool wasKilled();
    void increaseScore();
    void setKilledBy(int pId);
    int getKilledBy();
    bool getLeft();
    void setLeft(bool left);
    bool wasScoreSent();
    bool setScoreWasSent(bool sent);
    int getScore();

private:
    int id_;
    int score_;
    int killedBy_;
    bool left_;
    bool fired_;
    bool killed_;
    bool scoreWasSent_;
    CONNECTION* connection_;
    POSITION* position_;
    POSITION* initialPosition_;

};


#endif //SERVER_CLIENT_H
