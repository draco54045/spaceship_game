#pragma once
#include "Entity.h"

class Bullet : public Entity {
public:
    Bullet(float x, float y, Vec2 dir);
    void update(GameplayScene& g, float dt) override;
    void render(GameplayScene& g, SDL_Renderer* renderer, float dt) override;

private:
    Vec2 dir;
    float speed = 2000.0f;
    float life = 2.5f;
};
