#include <SDL2/SDL.h>
#include <stdbool.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

typedef struct {
    int x, y;
    int width, height;
} GameObject;

typedef struct {
    GameObject object;
    bool isEnemy;
} Entity;

Entity player;
Entity enemies[3];  // Example: Three enemies
Entity bullets[10]; // Example: Maximum of 10 bullets on the screen

bool initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Evony-like Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Initialize player
    player.object.x = SCREEN_WIDTH / 2;
    player.object.y = SCREEN_HEIGHT - 50;
    player.object.width = 50;
    player.object.height = 50;
    player.isEnemy = false;

    // Initialize enemies
    for (int i = 0; i < 3; ++i) {
        enemies[i].object.x = i * 200;
        enemies[i].object.y = 50;
        enemies[i].object.width = 50;
        enemies[i].object.height = 50;
        enemies[i].isEnemy = true;
    }

    return true;
}

void closeSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void renderGame() {
    // Clear the screen
    SDL_RenderClear(renderer);

    // Render player
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue color
    SDL_Rect playerRect = {player.object.x, player.object.y, player.object.width, player.object.height};
    SDL_RenderFillRect(renderer, &playerRect);

    // Render enemies
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
    for (int i = 0; i < 3; ++i) {
        SDL_Rect enemyRect = {enemies[i].object.x, enemies[i].object.y, enemies[i].object.width, enemies[i].object.height};
        SDL_RenderFillRect(renderer, &enemyRect);
    }

    // Render bullets
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color
    for (int i = 0; i < 10; ++i) {
        if (bullets[i].object.y > 0) {
            SDL_Rect bulletRect = {bullets[i].object.x, bullets[i].object.y, bullets[i].object.width, bullets[i].object.height};
            SDL_RenderFillRect(renderer, &bulletRect);
        }
    }

    // Update the screen
    SDL_RenderPresent(renderer);
}

void handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            SDL_Quit();
            exit(0);
        }
        else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_LEFT) {
                player.object.x -= 10;
            }
            else if (e.key.keysym.sym == SDLK_RIGHT) {
                player.object.x += 10;
            }
            else if (e.key.keysym.sym == SDLK_SPACE) {
                // Shoot a bullet
                for (int i = 0; i < 10; ++i) {
                    if (bullets[i].object.y <= 0) {
                        bullets[i].object.x = player.object.x;
                        bullets[i].object.y = player.object.y;
                        break;
                    }
                }
            }
        }
    }
}

void updateGame() {
    // Update enemies (example: move them down the screen)
    for (int i = 0; i < 3; ++i) {
        enemies[i].object.y += 1;
    }

    // Update bullets (example: move them up the screen)
    for (int i = 0; i < 10; ++i) {
        if (bullets[i].object.y > 0) {
            bullets[i].object.y -= 5;
        }
    }
}

int main(int argc, char* args[]) {
    if (!initializeSDL()) {
        return 1;
    }

    bool quit = false;

    while (!quit) {
        handleInput();
        updateGame();
        renderGame();
        SDL_Delay(16); // Cap the frame rate to around 60 frames per second
    }

    closeSDL();

    return 0;
}


