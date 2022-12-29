//
// Created by vrsp on 28. 12. 2022.
//

#include "Tank.h"

Tank::Tank() {
    this->speed_ = 100;
    this->reloadTime_ = 5;

    this->tankIcon_ = new SDL_Rect;
    this->tankIcon_->w = 20;
    this->tankIcon_->h = 40;
    this->direction_ = UP;

    this->bullet_ = new Bullet();

}

Tank::~Tank() {
    delete this->tankIcon_;
    delete this->bullet_;
    tankIcon_ = nullptr;
    bullet_ = nullptr;
}

SDL_Rect *Tank::getIcon() {
    return this->tankIcon_;
}

double Tank::getSpeed() const {
    return this->speed_;
}

double Tank::getReloadTime() {
    return this->reloadTime_;
}

void Tank::moveUp() {
    this->tankIcon_->y -= (int)this->speed_ / 20;
    this->direction_ = UP;
    this->tankIcon_->w = 20;
    this->tankIcon_->h = 40;
}

void Tank::moveDown() {
    this->tankIcon_->y += (int)this->speed_ / 20;
    this->direction_ = DOWN;
    this->tankIcon_->w = 20;
    this->tankIcon_->h = 40;
}

void Tank::moveLeft() {
    this->tankIcon_->x -= (int)this->speed_ / 20;
    this->direction_ = LEFT;
    this->tankIcon_->w = 40;
    this->tankIcon_->h = 20;
}

void Tank::moveRight() {
    this->tankIcon_->x += (int)this->speed_ / 20;
    this->direction_ = RIGHT;
    this->tankIcon_->w = 40;
    this->tankIcon_->h = 20;
}

void Tank::render(SDL_Renderer &renderer) {
    if (this->bullet_->wasFired())
        this->bullet_->render(renderer);
    SDL_RenderDrawRect(&renderer, this->tankIcon_);
}

void Tank::fire() {
    this->bullet_->shotBullet(this->tankIcon_->x, this->tankIcon_->y, this->direction_);
}

