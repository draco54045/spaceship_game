#pragma once
#include "Entity.h"
#include "Config.h"
#include <iostream>

class Player : public Entity {
public:
    Player(float x, float y);
    void handleInput(const Uint8* k, float dt);
    void update(Game& g, float dt) override;
    void render(Game& g, SDL_Renderer* renderer) override;

    float currentSpeed = 0.0f;
    float baseSpeed = 0.0f;
    float maxSpeed = 800.0f;
    float accel = 350.0f;
    float decel = 150.0f;
    Vec2 lastDir{0,0};

private:
    
};
