//
// Created by filip on 28. 12. 2022.
//
#include <SFML/Graphics.hpp>

enum DIRECTION {UP,
    DOWN,
    LEFT,
    RIGHT};

class Bullet{
public:
    Bullet();
    ~Bullet();

    bool wasFired() const;
    bool wasFiredAndSent() const;
    void setWasFiredAndSent();
    void shotBullet(float xPosition, float yPosition, DIRECTION direction);
    void moveBullet();
    void render(sf::RenderWindow& renderer);
private:
    sf::RectangleShape* bulletIcon_;
    float speed_;
    bool fired_;
    bool firedAndSent_;
    int diameter_;
    DIRECTION direction_;
};
