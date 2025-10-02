#include <SDL2/SDL.h>
#include <iostream>
#include <queue>
using namespace std;

// Window dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Square properties
int squareSize = 50;
int squareX = SCREEN_WIDTH / 2 - squareSize / 2;
int squareY = SCREEN_HEIGHT / 2 - squareSize / 2;
int speed = 5;


int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " 
                  << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Game",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: "
                  << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Renderer could not be created! SDL_Error: "
                  << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    SDL_Event e;
    // Game loop
    while (!quit) {
        
        // Event handling
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_w: squareY -= speed; break;
                    case SDLK_s: squareY += speed; break;
                    case SDLK_a: squareX -= speed; break;
                    case SDLK_d: squareX += speed; break;
                }
            }
        }
        // Keep square inside window bounds
        if (squareX < 0) squareX = 0;
        if (squareY < 0) squareY = 0;
        if (squareX + squareSize > SCREEN_WIDTH) squareX = SCREEN_WIDTH - squareSize;
        if (squareY + squareSize > SCREEN_HEIGHT) squareY = SCREEN_HEIGHT - squareSize;

        // Rendering
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
        SDL_RenderClear(renderer);

        SDL_Rect square = {squareX, squareY, squareSize, squareSize};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black square
        SDL_RenderFillRect(renderer, &square);

        SDL_RenderPresent(renderer);
        
        // Frame delay
        SDL_Delay(16); // 16 for ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
