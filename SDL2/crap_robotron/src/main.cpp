#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <array>
#include <vector>


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int NUM_ENEMIES = 10;
const int NUM_BULLETS = 256;

// enemy struct
struct Enemy {
  SDL_Rect rect;
  double speed;
  bool active = false;
};

std::array<Enemy, NUM_ENEMIES> enemies;
std::array<SDL_Rect, NUM_BULLETS> playerBullets;
std::array<SDL_Rect, NUM_BULLETS> enemyBullets;
SDL_Rect player;
int playerScore;

// move enemies toward player
void moveEnemiesTowardPlayer() {
  for (Enemy &enemy : enemies) {
    if (enemy.active) {
      double dx = player.x - enemy.rect.x;
      double dy = player.y - enemy.rect.y;
      double dist = sqrt(dx * dx + dy * dy);
      double moveX = (dx / dist) * enemy.speed;
      double moveY = (dy / dist) * enemy.speed;
      enemy.rect.x += moveX;
      enemy.rect.y += moveY;
    }
  }
}

// play sound
void playSound(Mix_Chunk *sound) {
  Mix_PlayChannel(-1, sound, 0);
}
bool checkCollision(const SDL_Rect &a, const SDL_Rect &b) {
  return (a.x + a.w > b.x &&
          a.x < b.x + b.w &&
          a.y + a.h > b.y &&
          a.y < b.y + b.h);
}

// find an inactive bullet
int findInactiveBullet(std::array<SDL_Rect, NUM_BULLETS> &bullets) {
  for (int i = 0; i < NUM_BULLETS; i++) {
    if (!bullets[i].w) {
      return i;
    }
  }
  return -1;
}

int main(int argc, char* argv[]) {
  
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
  
  SDL_Window* window = SDL_CreateWindow("Enemy Movement",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH, SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
  
 if(TTF_Init() != 0){
    printf("TTF_Init: %s\n", TTF_GetError());
    return 1;
  }
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
  // Initialize enemies
    for (int i = 0; i < NUM_ENEMIES; i++) {
    enemies[i].rect = {rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, 32, 32};
    enemies[i].speed = (rand() % 3) + 1;
    enemies[i].active = true;
  }



  // Initialize player
  player = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 32, 32};
  
  // Initialize sound
  Mix_Chunk *playerShootSound = Mix_LoadWAV("player_shoot.wav");
  Mix_Chunk *enemyShootSound = Mix_LoadWAV("enemy_shoot.wav");
  Mix_Chunk *playerHitSound = Mix_LoadWAV("player_hit.wav");
  Mix_Chunk *enemyHitSound = Mix_LoadWAV("enemy_hit.wav");

  // Load player, enemy bullet and enemy textures
  SDL_Surface* playerSurface = SDL_LoadBMP("player.bmp");
  SDL_Surface* enemySurface = SDL_LoadBMP("enemy.bmp");
  SDL_Surface* playerBulletSurface = SDL_LoadBMP("player_bullet.bmp");
  SDL_Surface* enemyBulletSurface = SDL_LoadBMP("enemy_bullet.bmp");
  SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
  SDL_Texture* enemyTexture = SDL_CreateTextureFromSurface(renderer, enemySurface);
  SDL_Texture* playerBulletTexture = SDL_CreateTextureFromSurface(renderer, playerBulletSurface);
  SDL_Texture* enemyBulletTexture = SDL_CreateTextureFromSurface(renderer, enemyBulletSurface);
  SDL_FreeSurface(playerSurface);
  SDL_FreeSurface(enemySurface);
  SDL_FreeSurface(playerBulletSurface);
  SDL_FreeSurface(enemyBulletSurface);

  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }

    moveEnemiesTowardPlayer();

    // Handle player input (shooting and movement)
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_UP]) {
      player.y -= 2;
    }
    if (keystate[SDL_SCANCODE_DOWN]) {
      player.y += 2;
    }
    if (keystate[SDL_SCANCODE_LEFT]) {
      player.x -= 2;
    }
    if (keystate[SDL_SCANCODE_RIGHT]) {
      player.x += 2;
    }
    if (keystate[SDL_SCANCODE_SPACE]) {
        int bulletIndex = findInactiveBullet(playerBullets);
        if (bulletIndex != -1) {
          playerBullets[bulletIndex] = {player.x, player.y, 2, 2};
          playSound(playerShootSound);
        }
    }

    // Handle enemy shooting
    for (Enemy &enemy : enemies) {
      if (enemy.active && rand() % 100 == 0) {
        int bulletIndex = findInactiveBullet(enemyBullets);
        if (bulletIndex != -1) {
          enemyBullets[bulletIndex] = {enemy.rect.x, enemy.rect.y, 2, 2};
          playSound(enemyShootSound);
        }
      }
    }

    // Handle collision detection
    for (Enemy &enemy : enemies) {
      if (enemy.active && checkCollision(player, enemy.rect)) {
        playSound(playerHitSound);
        // Handle player getting hit by enemy
      }
      for (int i = 0; i < NUM_BULLETS; i++) {
        if (playerBullets[i].w && checkCollision(playerBullets[i], enemy.rect)) {
          playSound(enemyHitSound);
          // Handle enemy getting hit by bullet
          playerBullets[i].w = 0;
          enemy.active = false;
          playerScore++;
        }
        if (enemyBullets[i].w && checkCollision(enemyBullets[i], player)) {
          playSound(playerHitSound);
          // Handle player getting hit by enemy bullet
          enemyBullets[i].w = 0;
        }
      }
    }


     SDL_Delay(40);


    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // Render player, enemies and bullets
    SDL_RenderCopy(renderer, playerTexture, NULL, &player);
    for (Enemy &enemy : enemies) {
      if (enemy.active) {
        SDL_RenderCopy(renderer, enemyTexture, NULL, &enemy.rect);
      }
    }
    for (int i = 0; i < NUM_BULLETS; i++) {
      if (playerBullets[i].w) {
        SDL_RenderCopy(renderer, enemyBulletTexture, NULL, &enemyBullets[i]);
      }
    }

    // update player score
    char playerScoreText[100];
    sprintf(playerScoreText, "Score: %d", playerScore);
    SDL_Color color = {255, 255, 255};
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, playerScoreText, color);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    int w, h;
    SDL_QueryTexture(scoreTexture, nullptr, nullptr, &w, &h);
    SDL_Rect scoreRect{5, 5, w, h};
    SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);
    
    // Update the screen
    SDL_RenderPresent(renderer);
  }

  // Clean up
  Mix_FreeChunk(playerShootSound);
  Mix_FreeChunk(enemyShootSound);
  Mix_FreeChunk(playerHitSound);
  Mix_CloseAudio();
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyTexture(playerTexture);
  SDL_DestroyTexture(enemyTexture);
  SDL_DestroyTexture(playerBulletTexture);
  SDL_DestroyTexture(enemyBulletTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

/*
In this example, I have used a pool for bullets, where each bullet is represented as an SDL_Rect in an array, 
and I check if the width of a bullet is set, meaning it's active, to render it on the screen. Also, I have 
used TTF_Init and TTF_RenderText_Solid to render player score on the screen. Keep in mind that TTF_Init() 
need to be call before TTF_RenderText_Solid() and TTF_Quit() after TTF_CloseFont(), and you will need to 
have SDL_ttf library to use TTF_* functions.

Please note that this is an example and you may need to adjust it for your specific use case.
      if (enemyBullets[i].w) {
       
*/


