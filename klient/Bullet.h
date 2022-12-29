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
