#include "Player.h"
#include "Game.h"
#include "Vec2.h"

Player::Player(float x, float y): Entity(x,y,48,48) { team = Team::Player; }

void Player::handleInput(const Uint8* k, float dt) {
    Vec2 vel{0,0};
    if(k[SDL_SCANCODE_W] || k[SDL_SCANCODE_UP]) vel.y -= 1;
    if(k[SDL_SCANCODE_S] || k[SDL_SCANCODE_DOWN]) vel.y += 1;
    if(k[SDL_SCANCODE_A] || k[SDL_SCANCODE_LEFT]) vel.x -= 1;
    if(k[SDL_SCANCODE_D] || k[SDL_SCANCODE_RIGHT]) vel.x += 1;
    if(length(vel) > 0) {
        vel = normalize(vel);
        if(dot(vel, lastDir) > 0.9f) {
            currentSpeed = std::min(maxSpeed, currentSpeed + accel * dt);
        }
        else{
            currentSpeed = std::max(baseSpeed, currentSpeed - decel * dt);
        }
        lastDir = vel;
    }
    else{
        currentSpeed = std::max(baseSpeed, currentSpeed - decel * dt);
    }
    pos = pos + vel * (currentSpeed * dt);
    if (length(vel) == 0 ){
        pos = pos + lastDir * (currentSpeed * dt);
    }
    if(pos.x < 0) pos.x = 0;
    if(pos.y < 0) pos.y = 0;
    if(pos.x + w > VIRTUAL_WORLD_W) pos.x = VIRTUAL_WORLD_W - w;
    if(pos.y + h > VIRTUAL_WORLD_H) pos.y = VIRTUAL_WORLD_H - h;
}

void Player::update(Game& /*g*/, float /*dt*/) {}

void Player::render(Game& g, SDL_Renderer* renderer) {
    SDL_Rect r{int(pos.x), int(pos.y), w, h};
    auto screen = g.camera.worldToScreen(r);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &screen);
}
