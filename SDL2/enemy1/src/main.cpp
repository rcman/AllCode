#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    float x, y;
    float speedX, speedY;
} Enemy;

void initializeEnemy(Enemy *enemy) {
    // Set initial position
    enemy->x = rand() % 800;  // Adjust the range based on your screen width
    enemy->y = 0;

    // Set random speed and direction
    enemy->speedX = (rand() % 5) - 2;  // Random speed between -2 and 2
    enemy->speedY = (rand() % 5) + 1;  // Random speed between 1 and 5
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window and renderer
    SDL_Window *window = SDL_CreateWindow("Space Shooting Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Seed for random number generation
    srand(time(NULL));

    // Create an array of enemies
    const int numEnemies = 5;
    Enemy enemies[numEnemies];

    // Initialize enemies
    for (int i = 0; i < numEnemies; ++i) {
        initializeEnemy(&enemies[i]);
    }

    // Game loop...
    int quit = 0;
    while (!quit) {
        // Handle events and input...
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Update enemy positions
        for (int i = 0; i < numEnemies; ++i) {
            enemies[i].x += enemies[i].speedX;
            enemies[i].y += enemies[i].speedY;

            // Check if enemy is out of screen, and reset its position
            if (enemies[i].y > 600) {  // Adjust the value based on your screen height
                initializeEnemy(&enemies[i]);
            }
        }

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Set the color to black
        SDL_RenderClear(renderer);

        // Render enemies
        for (int i = 0; i < numEnemies; ++i) {
            // Render each enemy as a red square (you can replace this with your own rendering code)
            SDL_Rect enemyRect = { (int)enemies[i].x, (int)enemies[i].y, 20, 20 };
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color for enemies
            SDL_RenderFillRect(renderer, &enemyRect);
        }

        // Present the renderer
        SDL_RenderPresent(renderer);

        // Delay to control the frame rate
        SDL_Delay(16);
    }

    // Cleanup code...
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


