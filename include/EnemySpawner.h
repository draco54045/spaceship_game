#pragma once
#include <SDL.h>
#include "Enemy.h"
#include "Player.h"
#include <cstdlib>
#include "Vec2.h"
#include <ctime>

class Player;
class GameplayScene;
class TextureManager;

class EnemySpawner{
public :
    EnemySpawner();
    ~EnemySpawner();
    void Set(GameplayScene& g, Player* player, TextureManager& tex);
    void update(float dt);
    Vec2 generatePosition();
    float spawnTimer;
    float spawnInterval = 2.0f;
    SDL_Texture* enemyTexture;
private:
    GameplayScene* game = nullptr;
    Player* plr = nullptr;
    TextureManager* texman = nullptr;
};