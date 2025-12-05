#include "Enemy.h"
#include "Game.h"

Enemy::Enemy(float x, float y, SDL_Texture* tex) : Entity(x, y, 64, 64){
    team = Team::Enemy;
    health = 50.0f;
    texture = tex;
}

void Enemy::update(GameplayScene& g, float dt){
    Vec2 playerDir = {g.player->pos.x, g.player->pos.y};
    followDir = {playerDir.x - pos.x, playerDir.y - pos.y};
    followDir = normalize(followDir);
    float rad = this->rotation * (M_PI / 180.0f);
    forward = {cosf(rad), sinf(rad)};
    currentSpeed = 300.0f;
    pos = pos + forward * (currentSpeed * dt);
    this->hitbox = aabb();
}

void Enemy::render(GameplayScene& g, SDL_Renderer* renderer, float dt){
    SDL_Rect rect = aabb();
    auto screen = g.camera.worldToScreen(rect);  
    float targetAngle = atan2(followDir.y, followDir.x) * (180.0f / M_PI);
    float delta = fmodf(targetAngle - this->rotation + 540.0f, 360.0f) - 180.0f;
    const float turnAccel = 200.0f;
    rotVel += delta * turnAccel * dt;
    const float turnDamping = 0.8f;
    rotVel *= turnDamping;
    const float maxRotVel = 360.0f;
    rotVel = clamp(rotVel, -maxRotVel, maxRotVel);
    this->rotation += rotVel * dt;
    if (this->rotation > 180.0f) this->rotation -= 360.0f;
    if (this->rotation < -180.0f) this->rotation += 360.0f;
    SDL_RenderCopyEx(renderer, texture, NULL, &screen, this->rotation+90, NULL, SDL_FLIP_NONE);
    
    //draw hitbox
    auto hb = g.camera.worldToScreen(this->hitbox);
    SDL_SetRenderDrawColor(renderer, 255,0,0,255);
    SDL_RenderDrawRect(renderer, &hb);
}