#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <list>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Data structure for enemies
struct Enemy {
    int x, y;
    int speed;
    bool alive;
    int shooting_frequency;
    int last_shoot_time;
};
std::list<Enemy> enemies;

// Data structure for bullets
struct Bullet {
    int x, y;
    bool active;
};
std::list<Bullet> bullets;
std::list<Bullet> e_bullets;

SDL_Rect player;

void fireBullet(int x, int y) {
    for (auto it = bullets.begin(); it != bullets.end(); it++) {
        if (!it->active) {
            it->active = true;
            it->x = x;
            it->y = y;
            Mix_PlayChannel(-1, fireSound, 0);
            break;
        }
    }
}

void fireEnemyBullet(int x, int y) {
    for (auto it = e_bullets.begin(); it != e_bullets.end(); it++) {
        if (!it->active) {
            it->active = true;
            it->x = x;
            it->y = y;
            Mix_PlayChannel(-1, fireSound, 0);
            break;
        }
    }
}

void moveEnemiesTowardPlayer() {
    for (auto it = enemies.begin(); it != enemies.end(); it++) {
        if (!it->alive) continue; //skip if enemy is dead
        // Move the enemy towards the player at a different speed
        it->x -= (player.x - it->x) / it->speed;
        it->y -= (player.y - it->y) / it->speed;
        // Randomly shoot towards the player
        int current_time = SDL_GetTicks();
        if ((current_time - it->last_shoot_time) > it->shooting_frequency) {
            fireEnemyBullet(it->x, it->y);
            it->last_shoot_time = current_time;
        }
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

void moveEBullets() {
    for (auto it = e_bullets.begin(); it != e_bullets.end(); it++) {
        if (!it->active) continue; //skip if bullet is inactive
        // Move the bullet downwards
        it->y += 5;
        if (it->y > SCREEN_HEIGHT) {
            it->active = false;
        }
    }
    e_bullets.remove_if([](const Bullet& b) { return !b.active; });
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
                Mix_PlayChannel(-1, hitSound, 0);
           
            }
        }
    }
    enemies.remove_if([](const Enemy& e) { return !e.alive; });
}

void checkEBulletHits() {
    for (auto bulletIt = e_bullets.begin(); bulletIt != e_bullets.end(); bulletIt++) {
        if (!bulletIt->active) continue; //skip if bullet is inactive
        // Check if the bullet hits the player
        if (bulletIt->x >= player.x && bulletIt->x <= player.x + player.w &&
            bulletIt->y >= player.y && bulletIt->y <= player.y + player.h) {
            player.x = SCREEN_WIDTH / 2;
            player.y = SCREEN_HEIGHT / 2;
            bulletIt->active = false;
            Mix_PlayChannel(-1, hitSound, 0);
        }
    }
    e_bullets.remove_if([](const Bullet& b) { return !b.active; });
}

int main() {
    srand(time(NULL));
    // Initialize SDL, Mixer and create a window
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Chunk* fireSound = Mix_LoadWAV("fire.wav");
    Mix_Chunk* hitSound = Mix_LoadWAV("hit.wav");
    SDL_Window* window = SDL_CreateWindow("Enemies", SDL_WINDOWPOS_UNDEFINED,
                 SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

 // Load the player and enemy bitmaps
    SDL_Surface* playerSurface = SDL_LoadBMP("player.bmp");
    SDL_Surface* enemySurface = SDL_LoadBMP("enemy.bmp");
    SDL_Surface* bulletSurface = SDL_LoadBMP("bullet.bmp");
    SDL_Surface* e_bulletSurface = SDL_LoadBMP("e_bullet.bmp");

    // Set up the player and enemies
    player = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 32, 32};
    for (int i = 0; i < 32; i++) {
        int x = rand() % (SCREEN_WIDTH - 32);
        int y = rand() % (SCREEN_HEIGHT - 32);
        int speed = (rand() % 10) + 5;
        int shooting_frequency = (rand() % 1000) + 500;
        enemies.push_back({x, y, speed, true, shooting_frequency, 0});
    }
    for (int i = 0; i < 32; i++) {
        bullets.push_back({0, 0, false});
        e_bullets.push_back({0, 0, false});
    }

    // Main game loop
    bool running = true;
    while (running) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        player.y -= 5;
                        break;
                    case SDLK_DOWN:
                        player.y += 5;
                        break;
                    case SDLK_LEFT:
                        player.x -= 5;
                        break;
                    case SDLK_RIGHT:
                        player.x += 5;
                        break;
                    case SDLK_SPACE:
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        fireBullet(x, y);
                        break;
                    default:
                        break;
                }
            }
        }
        moveEnemiesTowardPlayer();
        moveBullets();
        moveEBullets();
        checkBulletHits();
        checkEBulletHits();
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
        for (auto it = e_bullets.begin(); it != e_bullets.end(); it++) {
            if (it->active) {
                SDL_Rect rect = {it->x, it->y, 5, 5};
                SDL_BlitSurface(e_bulletSurface, NULL, screenSurface, &rect);
            }
        }
        SDL_UpdateWindowSurface(window);
    }
    Mix_FreeChunk(fireSound);
    Mix_FreeChunk(hitSound);
    Mix_CloseAudio();
    SDL_FreeSurface(playerSurface);
        SDL_FreeSurface(enemySurface);
    SDL_FreeSurface(bulletSurface);
    SDL_FreeSurface(e_bulletSurface);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


