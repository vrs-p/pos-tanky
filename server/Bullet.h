//
// Created by vrsp on 30. 12. 2022.
//

#ifndef SERVER_BULLET_H
#define SERVER_BULLET_H


enum DIRECTION {UP,
    DOWN,
    LEFT,
    RIGHT};

class Bullet{
public:
    Bullet();
    ~Bullet();

    bool wasFired() const;
    void shotBullet(int xPosition, int yPosition, DIRECTION direction);
    void moveBullet();
    void render(sf::RenderWindow& renderer);
private:
    sf::RectangleShape* bulletIcon_;
    float speed_;
    bool fired_;
    int diameter_;
    DIRECTION direction_;
};


#endif //SERVER_BULLET_H
