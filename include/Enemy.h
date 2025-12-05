#pragma once
#include "Entity.h"
#include <SDL.h>

class GameplayScene;

enum class EnemyState { Chase, Run, Avoid, Group };

class Enemy : public Entity {
public :
    Enemy(float x, float y, SDL_Texture* tex);
    virtual ~Enemy() = default;

    void update(GameplayScene& g, float dt) override;
    void render(GameplayScene& g, SDL_Renderer* renderer, float dt) override;

    float currentSpeed = 0.0f;
    float maxSpeed = 200.0f;
    float accel = 200.0f; 
    float decel = 25.0f;
    Vec2 forward{0,0};
    Vec2 followDir{1,0};
    Vec2 velocity{0,0};
    SDL_Texture* texture = nullptr;
    float rotVel = 0.0f;
    EnemyState state = EnemyState::Chase;
};