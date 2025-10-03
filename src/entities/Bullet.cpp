#include "Bullet.h"
#include "Game.h"

Bullet::Bullet(float x, float y, Vec2 d): Entity(x,y,8,8), dir(normalize(d)) { team = Team::Bullet; }

void Bullet::update(Game& /*g*/, float dt) {
    pos = pos + dir * (speed * dt);
    life -= dt;
    if(life <= 0) alive = false;
    if(pos.x < -50 || pos.y < -50 || pos.x > VIRTUAL_WORLD_W+50 || pos.y > VIRTUAL_WORLD_H+50) alive = false;
}

void Bullet::render(Game& g, SDL_Renderer* renderer) {
    SDL_Rect r{int(pos.x), int(pos.y), w, h};
    auto screen = g.camera.worldToScreen(r);
    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
    SDL_RenderFillRect(renderer, &screen);
}
