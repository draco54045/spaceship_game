#pragma once
#include <iostream>
#include <vector>
#include "Player.h"

class Player;
class GameplayScene;

struct UIPlayerData {
    std::string speed;
    std::string direction;
    std::string mouseVector;
    std::string fps;
};

struct UIGameData {
    std::string fps;
};

class UI {
public:
    UI();
    ~UI();

    void Set(Player* plr, GameplayScene& g);
    UIPlayerData getPlayerData();
    UIGameData getGameData();
    Player* player = nullptr;
    GameplayScene* game = nullptr;
};