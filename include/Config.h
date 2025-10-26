#pragma once

static const int WINDOW_W = 1280;
static const int WINDOW_H = 800;
static const int VIRTUAL_WORLD_W = 4096;
static const int VIRTUAL_WORLD_H = 4096;

class Config {
public:
    int windowWidth;
    int windowHeight;
    float scaleX;
    float scaleY;

    static Config& get() {
        static Config instance;
        return instance;
    }

    void updateScale(int baseWidth = 1280, int baseHeight = 800) {
        scaleX = (float)windowWidth / baseWidth;
        scaleY = (float)windowHeight / baseHeight;
    }

private:
    Config() : windowWidth(1280), windowHeight(800) {
        updateScale();
    }
};
