#include "Player.h"
#include "Game.h"
#include "Vec2.h"

Player::Player(float x, float y): Entity(x,y,64,64) { team = Team::Player; }

void Player::handleInput(const Uint8* k, float dt, Game& g) {
    SDL_Rect r{int(pos.x), int(pos.y), w, h};
    auto screen = g.camera.worldToScreen(r);
    int mx = 0,my = 0; 
    SDL_GetMouseState(&mx,&my);
    float cx = screen.x + screen.w / 2.0f;
    float cy = screen.y + screen.h / 2.0f;
    dir.x = mx-cx;
    dir.y = my-cy;
    float dist = length(dir);
    const float enterRadius = deadZoneRadius * 1.1f;
    const float exitRadius  = deadZoneRadius * 0.9f;
    static bool insideDeadZone = false;
    Vec2 vel{0,0};
    if(k[SDL_SCANCODE_W] || k[SDL_SCANCODE_UP]) {vel.y = dir.y; vel.x = dir.x; currentSpeed = std::min(maxSpeed, currentSpeed + accel * dt);}
    //if(k[SDL_SCANCODE_S] || k[SDL_SCANCODE_DOWN]) {}
    //if(k[SDL_SCANCODE_A] || k[SDL_SCANCODE_LEFT]) vel.x -= 1;
    //if(k[SDL_SCANCODE_D] || k[SDL_SCANCODE_RIGHT]) vel.x += 1;
    if (insideDeadZone) {
        if (dist > enterRadius) {
            insideDeadZone = false;
            Vec2 newDir = normalize(dir);
            dir = lerpVec(lastDir, newDir, 10.0f * dt);
            lastDir = normalize(dir);
        } 
        else {
            dir = lastDir;
        }
    }
    else {
        if (dist < exitRadius) {
            insideDeadZone = true;
            dir = lastDir;
        } 
        else {
            Vec2 newDir = normalize(dir);
            dir = lerpVec(lastDir, newDir, 10.0f * dt);
            lastDir = normalize(dir);
        }
    }
}

void Player::update(Game& g, float dt) {
    currentSpeed = std::max(baseSpeed, currentSpeed - decel * dt);
    pos = pos + dir * (currentSpeed * dt);
    if(pos.x < 0) pos.x = 0;
    if(pos.y < 0) pos.y = 0;
    if(pos.x + w > VIRTUAL_WORLD_W) pos.x = VIRTUAL_WORLD_W - w;
    if(pos.y + h > VIRTUAL_WORLD_H) pos.y = VIRTUAL_WORLD_H - h;
}

void Player::render(Game& g, SDL_Renderer* renderer, float dt) {
    SDL_Rect r{int(pos.x), int(pos.y), w, h};
    auto screen = g.camera.worldToScreen(r);
    int mx = 0,my = 0; 
    SDL_GetMouseState(&mx,&my);
    float cx = screen.x + screen.w / 2.0f;
    float cy = screen.y + screen.h / 2.0f;
    float targetAngle = atan2(dir.y, dir.x) * (180.0f / M_PI);
    float angleDiff = fabsf(fmodf(targetAngle - rotation + 540.0f, 360.0f) - 180.0f);
    if (angleDiff > 120.0f) {
        currentSpeed *= 0.7f;
    }
    rotation = lerpAngle(rotation, targetAngle, 10.0f * dt);
    SDL_RenderCopyEx(renderer, texture, NULL, &screen, rotation+90, NULL, SDL_FLIP_NONE);
}
