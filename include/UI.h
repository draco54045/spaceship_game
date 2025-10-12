#pragma once
#include <iostream>
#include <vector>
#include "Player.h"

class Player;

struct UIData {
    std::string speed;
    std::string direction;
};

class UI {
public:
    UI();
    ~UI();

    void Set(Player* plr);
    UIData getData();
    Player* player = nullptr;
};