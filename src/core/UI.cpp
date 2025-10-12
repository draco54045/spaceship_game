#include "UI.h"

UI::UI(){}
UI::~UI(){}

void UI::Set(Player* plr){
    player = plr;
}

UIData UI::getData(){
    return { 
        std::to_string(player->currentSpeed),
        "[" + std::to_string(player->lastDir.x) + ", " + std::to_string(player->lastDir.y) + "]"
    };
}