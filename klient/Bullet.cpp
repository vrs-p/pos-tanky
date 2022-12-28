//
// Created by filip on 28. 12. 2022.
//
#include "Bullet.h"

Bullet::Bullet() {
    this->speed_ = 100;
    this->fired_ = false;
    this->bulletIcon_ = new SDL_Rect;
}

Bullet::~Bullet() {
    delete this->bulletIcon_;
    this->bulletIcon_ = nullptr;
}

void Bullet::shotBullet(int xPosition, int yPosition, DIRECTION direction) {
    this->bulletIcon_->w = 3;
    this->bulletIcon_->h = 3;
    this->bulletIcon_->x = xPosition;
    this->bulletIcon_->y = yPosition;
    this->direction_ = direction;
    this->fired_ = true;
}

void Bullet::render(SDL_Renderer &renderer) {
    if (this->bulletIcon_->x > 0 && this->bulletIcon_->x < 800 &&
        this->bulletIcon_->y > 0 && this->bulletIcon_->y < 800 &&
        this->fired_)
    {
        moveBullet();
        SDL_RenderDrawRect(&renderer, this->bulletIcon_);
    } else {
        this->fired_ = false;
    }
}

void Bullet::moveBullet() {
    if (this->direction_ == UP){
        this->bulletIcon_->y -= (int)this->speed_ / 20;
    } else if (this->direction_ == DOWN) {
        this->bulletIcon_->y += (int)this->speed_ / 20;
    } else if (this->direction_ == LEFT) {
        this->bulletIcon_->x -= (int)this->speed_ / 20;
    } else {
        this->bulletIcon_->x += (int)this->speed_ / 20;
    }
}

bool Bullet::wasFired() {
    return this->fired_;
}
