#include "Player.h"
#include "Game.h"
#include "Vec2.h"

Player::Player(float x, float y): Entity(x,y,64,64) { team = Team::Player; }

void Player::handleInput(const Uint8* k, float dt, GameplayScene& g) {
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
            //Vec2 newDir = normalize(dir);
            //dir = lerpVec(lastDir, newDir, 10.0f * dt);
            //lastDir = normalize(dir);
            dir = normalize(dir);
            lastDir = dir;
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

void Player::update(GameplayScene& g, float dt) {
    currentSpeed = std::max(baseSpeed, currentSpeed - decel * dt);
    float rad = this->rotation * (M_PI / 180.0f);
    forward = {cosf(rad), sinf(rad)};
    pos = pos + forward * (currentSpeed * dt);
    //pos = pos + dir * (currentSpeed * dt);
    if(pos.x < 0) pos.x = 0;
    if(pos.y < 0) pos.y = 0;
    if(pos.x + w > VIRTUAL_WORLD_W) pos.x = VIRTUAL_WORLD_W - w;
    if(pos.y + h > VIRTUAL_WORLD_H) pos.y = VIRTUAL_WORLD_H - h;
}

void Player::render(GameplayScene& g, SDL_Renderer* renderer, float dt) {
    SDL_Rect r{int(pos.x), int(pos.y), w, h};
    auto screen = g.camera.worldToScreen(r);
    int mx = 0,my = 0; 
    SDL_GetMouseState(&mx,&my);
    float cx = screen.x + screen.w / 2.0f;
    float cy = screen.y + screen.h / 2.0f;
    //float targetAngle = atan2(dir.y, dir.x) * (180.0f / M_PI);
    //float angleDiff = fabsf(fmodf(targetAngle - this->rotation + 540.0f, 360.0f) - 180.0f);
    //if (angleDiff > 120.0f) {
    //    currentSpeed *= 0.7f;
    //}
    //this->rotation = lerpAngle(this->rotation, targetAngle, clamp(25.0f * dt, 0.0f, 1.0f));
    float targetAngle = atan2(dir.y, dir.x) * (180.0f / M_PI);
    float delta = fmodf(targetAngle - this->rotation + 540.0f, 360.0f) - 180.0f;
    const float turnAccel = 200.0f;
    rotVel += delta * turnAccel * dt;
    const float turnDamping = 0.8f;
    rotVel *= turnDamping;
    const float maxRotVel = 360.0f;
    rotVel = clamp(rotVel, -maxRotVel, maxRotVel);
    this->rotation += rotVel * dt;
    //this->rotation += delta * clamp(25.0f * dt, 0.0f, 1.0f);   
    if (this->rotation > 180.0f) this->rotation -= 360.0f;
    if (this->rotation < -180.0f) this->rotation += 360.0f;
    SDL_RenderCopyEx(renderer, texture, NULL, &screen, this->rotation+90, NULL, SDL_FLIP_NONE);
}
