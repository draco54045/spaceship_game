#pragma once
#include <SDL.h>
#include "Vec2.h"

class GameplayScene;

enum class Team { Neutral, Player, Enemy, Bullet, PlayerBullet };

class Entity {
public:
    Entity(float x, float y, int w, int h);
    virtual ~Entity() = default;
    virtual void update(GameplayScene& g, float dt) {}
    virtual void render(GameplayScene& g, SDL_Renderer* renderer, float dt);
    virtual void onCollision(Entity& ent) {}
    SDL_Rect aabb() const;

    Vec2 pos;
    int w, h;
    float health;
    SDL_Rect hitbox;
    double rotation;
    bool alive = true;
    Team team = Team::Neutral;
};
