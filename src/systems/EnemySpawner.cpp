#include "EnemySpawner.h"
#include "Game.h"

EnemySpawner::EnemySpawner(){
    srand(time(0));
}
EnemySpawner::~EnemySpawner(){}

void EnemySpawner::Set(GameplayScene& g, Player* player, TextureManager& tex){
    game = &g;
    plr = player;
    texman = &tex;
    enemyTexture = texman->load("./assets/graphics/enemy_test.png");
    game->entities->create<Enemy>(VIRTUAL_WORLD_W/2.0f + 300, VIRTUAL_WORLD_H/2.0f + 200, enemyTexture);
}

void EnemySpawner::update(float dt){
    Vec2 position = generatePosition();
    spawnTimer += dt;
    if(spawnTimer >= spawnInterval){
        game->entities->create<Enemy>(position.x, position.y, enemyTexture);
        spawnTimer = 0.0f;
    }
}

Vec2 EnemySpawner::generatePosition(){
    Vec2 position;
    int randomX = (rand() % 301) + 150;
    int randomY = (rand() % 301) + 150;
    position.x = plr->pos.x - randomX;
    position.y = plr->pos.y - randomY;
    return position;
}