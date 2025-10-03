#pragma once
#include <SDL.h>
#include "Vec2.h"

class Game;

enum class Team { Neutral, Player, Enemy, Bullet };

class Entity {
public:
    Entity(float x, float y, int w, int h);
    virtual ~Entity() = default;
    virtual void update(Game& g, float dt) {}
    virtual void render(Game& g, SDL_Renderer* renderer);
    SDL_Rect aabb() const;

    Vec2 pos;
    int w, h;
    bool alive = true;
    Team team = Team::Neutral;
};
