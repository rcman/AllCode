#include <SDL.h>
#include <list>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Data structure for enemies
struct Enemy {
    int x, y;
    int speed;
    bool alive;
};
std::list<Enemy> enemies;

// Data structure for bullets
struct Bullet {
    int x, y;
    bool active;
};
std::list<Bullet> bullets;

SDL_Rect player;

void moveEnemiesTowardPlayer() {
    for (auto it = enemies.begin(); it != enemies.end(); it++) {
        if (!it->alive) continue; //skip if enemy is dead
        // Move the enemy towards the player at a different speed
        it->x -= (player.x - it->x) / it->speed;
        it->y -= (player.y - it->y) / it->speed;
    }
}

void moveBullets() {
    for (auto it = bullets.begin(); it != bullets.end(); it++) {
        if (!it->active) continue; //skip if bullet is inactive
        // Move the bullet upwards
        it->y -= 5;
        if (it->y < 0) {
            it->active = false;
        }
    }
    bullets.remove_if([](const Bullet& b) { return !b.active; });
}

void checkBulletHits() {
    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); enemyIt++) {
        if (!enemyIt->alive) continue; //skip if enemy is dead
        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); bulletIt++) {
            if (!bulletIt->active) continue; //skip if bullet is inactive
            // Check if the bullet hits the enemy
            if (bulletIt->x >= enemyIt->x && bulletIt->x <= enemyIt->x + 32 &&
                bulletIt->y >= enemyIt->y && bulletIt->y <= enemyIt->y + 32) {
                enemyIt->alive = false;
                bulletIt->active = false;
                // Mix_PlayChannel(-1, hitSound, 0);
            }
        }
    }
    enemies.remove_if([](const Enemy& e) { return !e.alive; });
}

int main() {
    // Initialize SDL, create a window
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Enemies", SDL_WINDOWPOS_UNDEFINED,
                 SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    // Load the player and enemy bitmaps
    SDL_Surface* playerSurface = SDL_LoadBMP("player.bmp");
    SDL_Surface* enemySurface = SDL_LoadBMP("enemy.bmp");
    SDL_Surface* bulletSurface = SDL_LoadBMP("bullet.bmp");

    // Set up the player and enemies
    player = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 32, 32};
    for (int i = 0; i < 32; i++) {
        int x = rand() % (SCREEN_WIDTH - 32);
        int y = rand() % (SCREEN_HEIGHT - 32);
        int speed = (rand() % 10) + 5;
        enemies.push_back({x, y, speed, true});
    }
    for (int i = 0; i < 32; i++) {
        bullets.push_back({0, 0, false});
    }
    // Main game loop
    bool running = true;
    while (running) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                fireBullet(x, y);
            }
        }
        moveEnemiesTowardPlayer();
        moveBullets();
        checkBulletHits();

        // Draw the player, enemies, and bullets
        SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
        SDL_BlitSurface(playerSurface, NULL, screenSurface, &player);
        for (auto it = enemies.begin(); it != enemies.end(); it++) {
            if (it->alive) {
                                SDL_Rect rect = {it->x, it->y, 32, 32};
                SDL_BlitSurface(enemySurface, NULL, screenSurface, &rect);
            }
        }
        for (auto it = bullets.begin(); it != bullets.end(); it++) {
            if (it->active) {
                SDL_Rect rect = {it->x, it->y, 5, 5};
                SDL_BlitSurface(bulletSurface, NULL, screenSurface, &rect);
            }
        }
        SDL_UpdateWindowSurface(window);
    }

    // Clean up
    SDL_FreeSurface(playerSurface);
    SDL_FreeSurface(enemySurface);
    SDL_FreeSurface(bulletSurface);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


