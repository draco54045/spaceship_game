#pragma once
#include "Entity.h"
#include "Config.h"

class Player : public Entity {
public:
    Player(float x, float y);
    void handleInput(const Uint8* k, float dt);
    void update(Game& g, float dt) override;
    void render(Game& g, SDL_Renderer* renderer) override;

private:
    float speed = 300.0f;
};
