#pragma once

class GameState {
public:
    bool paused;
    bool settings;

    static GameState& get() {
        static GameState instance;
        return instance;
    }
private:
    GameState(): paused(false), settings(false){}
};