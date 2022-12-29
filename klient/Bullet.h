//
// Created by filip on 28. 12. 2022.
//
#include "SDL.h"

enum DIRECTION {UP,
    DOWN,
    LEFT,
    RIGHT};

class Bullet{
public:
    Bullet();
    ~Bullet();

    bool wasFired();
    void shotBullet(int xPosition, int yPosition, DIRECTION direction);
    void moveBullet();
    void render(SDL_Renderer& renderer);
private:
    SDL_Rect* bulletIcon_;
    double speed_;
    bool fired_;
    DIRECTION direction_;
};
