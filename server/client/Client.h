//
// Created by filip on 30. 12. 2022.
//
#include <SFML/Network.hpp>
#include <mutex>
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
    Client(int id, std::string pName, float xPosition, float yPosition, DIRECTION direction, unsigned short port, sf::IpAddress ipAddress);
    ~Client();


    POSITION* getPosition();
    CONNECTION* getConnection();

    bool getFired();
    bool wasKilled();
    bool getLeft();
    bool wasScoreSent();

    int getClientId();
    int getKilledBy();
    int getScore();

    void updatePosition(float xPosition, float yPosition, DIRECTION direction);
    void setInitialPosition(float xPosition, float yPosition, DIRECTION direction);
    void setFired(bool fired);
    void resetPosition();
    void killed();
    void setKilledBy(int pId);
    void setLeft(bool left);
    void increaseScore();
    void setScoreWasSent(bool sent);

    std::string getPlayerName();

    void lockMutex();
    void unlockMutex();

private:
    int id_;
    int score_;
    int killedBy_;
    bool left_;
    bool fired_;
    bool killed_;
    bool scoreWasSent_;
    std::string playerName_;
    CONNECTION* connection_;
    POSITION* position_;
    POSITION* initialPosition_;

    std::mutex mutex_;

};


#endif //SERVER_CLIENT_H
