#pragma once
#include "Entity.h"

class Bullet : public Entity {
public:
    Bullet(float x, float y, Vec2 dir);
    void update(Game& g, float dt) override;
    void render(Game& g, SDL_Renderer* renderer) override;

private:
    Vec2 dir;
    float speed = 600.0f;
    float life = 2.5f;
};
