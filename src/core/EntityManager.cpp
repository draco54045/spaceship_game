#include "EntityManager.h"
#include "Game.h"

void EntityManager::updateAll(GameplayScene& g, float dt) {
    for(auto &e: entities){ 
        if(e->health <= 0){ 
            if (e->team == Team::Enemy) { g.score += 1; } 
            e->alive = false;
        }
        if(e->alive) e->update(g, dt); 
    }
    entities.erase(std::remove_if(entities.begin(), entities.end(),
        [](const std::unique_ptr<Entity>& p){ return !p->alive; }),
        entities.end());
}

void EntityManager::renderAll(GameplayScene& g, SDL_Renderer* renderer, float dt) {
    for(auto &e: entities) if(e->alive) e->render(g, renderer, dt);
}

void EntityManager::checkCollisions(GameplayScene& g){
    for (size_t i = 0; i < entities.size(); ++i){
        Entity* a = entities[i].get();
        if (!a->alive) continue;
        SDL_Rect aHitbox = a->hitbox;
        for (size_t j = i + 1; j < entities.size(); ++j){
            Entity* b = entities[j].get();
            if(!b->alive) continue;
            SDL_Rect bHitbox = b->hitbox;
            if (SDL_HasIntersection(&aHitbox, &bHitbox)){
                a->onCollision(*b);
                b->onCollision(*a);
                //if(a->team == Team::Enemy && b->team == Team::Bullet){
                    //std::cout << "Hit an enemy" << std::endl;
                    //a->health -= 10;
                    //b->alive = false;
                //}
                //else if(a->team == Team::Player && b->team == Team::Enemy){
                    //std::cout << "Colliding with an enemy" << std::endl;
                //}
            }
        }
    }
}