#pragma once
#include "Entity.h"
#include "Config.h"
#include <iostream>

class GameplayScene;

class Player : public Entity {
public:
    Player(float x, float y, float hlth);
    void handleInput(const Uint8* k, float dt, GameplayScene& g);
    void update(GameplayScene& g, float dt) override;
    void render(GameplayScene& g, SDL_Renderer* renderer, float dt) override;

    float currentSpeed = 0.0f;
    float baseSpeed = 0.0f;
    float maxSpeed = 600.0f;
    float accel = 200.0f;
    float decel = 200.0f;
    Vec2 lastDir{0,0};
    Vec2 dir{0,0};
    Vec2 forward{1,0};
    float deadZoneRadius = 48.0f;
    SDL_Texture* texture = nullptr;
    //double rotation = 10;
    float rotVel = 0.0f;

    //SDL_Rect hitbox;
private:
    
};
