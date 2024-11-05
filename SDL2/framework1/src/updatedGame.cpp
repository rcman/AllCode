#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Map.h"

GameObject* player;
GameObject* enemy;
Map* map;

SDL_Renderer* Game::renderer = nullptr;

Game::Game() {}
Game::~Game() {
    // Properly free the allocated objects
    delete player;
    delete enemy;
    delete map;
}

void Game::init(const char* title, int width, int height, bool fullscreen) {
    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        if (window) {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                isRunning = true;
            }
        }
    }
    if (!window || !renderer) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        isRunning = false;
    }

    player = new GameObject("assets/player.bmp", 10, 10);
    enemy = new GameObject("assets/enemy.bmp", 50, 50);
    map = new Map();
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
