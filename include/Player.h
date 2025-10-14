#pragma once
#include "Entity.h"
#include "Config.h"
#include <iostream>


class Player : public Entity {
public:
    Player(float x, float y);
    void handleInput(const Uint8* k, float dt, Game& g);
    void update(Game& g, float dt) override;
    void render(Game& g, SDL_Renderer* renderer, float dt) override;
    float lerpAngle(float a, float b, float t);

    float currentSpeed = 0.0f;
    float baseSpeed = 0.0f;
    float maxSpeed = 400.0f;
    float accel = 200.0f;
    float decel = 25.0f;
    Vec2 lastDir{0,0};
    Vec2 dir{0,0};
    float deadZoneRadius = 48.0f;
    SDL_Texture* texture = nullptr;
    double rotation = 0.0;
private:
    
};
