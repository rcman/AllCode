#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <cstdlib>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;

typedef struct {
    int x, y;
    int width, height;
} Entity;

typedef struct {
    Entity object;
    bool hit; // New flag to track whether the bullet has hit something
} Bullet;

Entity player;
Entity enemies[3];  // Example: Three enemies
Bullet bullets[10]; // Example: Maximum of 10 bullets on the screen

typedef struct {
    Entity object;
    int value; // The value to display on the enemy
} Text;

Text enemyTexts[3]; // Text for displaying hit count

// New structure for quit text
typedef struct {
    Entity object;
    std::string message;
} QuitText;

QuitText quitText;

bool initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
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

    font = TTF_OpenFont("arial.ttf", 24); // Replace with the path to your TrueType font file
    if (!font) {
        printf("Font could not be loaded! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    // Initialize player
    player.x = SCREEN_WIDTH / 2;
    player.y = SCREEN_HEIGHT - 50;
    player.width = 50;
    player.height = 50;

    // Initialize enemies
    for (int i = 0; i < 3; ++i) {
        enemies[i].x = i * 200;
        enemies[i].y = 50;
        enemies[i].width = 50;
        enemies[i].height = 50;
    }

    // Initialize bullets
    for (int i = 0; i < 10; ++i) {
        bullets[i].object.x = 0;
        bullets[i].object.y = 0;
        bullets[i].object.width = 5;  // Set the width of the bullet
        bullets[i].object.height = 10; // Set the height of the bullet
        bullets[i].hit = false; // Initialize the hit flag
    }

    // Initialize text objects for displaying hit count
    for (int i = 0; i < 3; ++i) {
        enemyTexts[i].object.width = 50; // Width of the text box
        enemyTexts[i].object.height = 20; // Height of the text box
        enemyTexts[i].object.x = enemies[i].x;
        enemyTexts[i].object.y = enemies[i].y - enemyTexts[i].object.height;
        enemyTexts[i].value = 3; // Initial hit count
    }

    // Initialize quit text
    quitText.object.x = 10;
    quitText.object.y = 10;
    quitText.object.width = 200;
    quitText.object.height = 20;
    quitText.message = "Press Q to Quit";

    return true;
}

void closeSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

void renderText(Text* text) {
    SDL_Color textColor = {255, 255, 255}; // White color
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, std::to_string(text->value).c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect destRect = {text->object.x, text->object.y, text->object.width, text->object.height};
    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void renderGame() {
    // Clear the screen with a black background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render player
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue color
    SDL_Rect playerRect = {player.x, player.y, player.width, player.height};
    SDL_RenderFillRect(renderer, &playerRect);

    // Render enemies and associated hit count text
    for (int i = 0; i < 3; ++i) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
        SDL_Rect enemyRect = {enemies[i].x, enemies[i].y, enemies[i].width, enemies[i].height};
        SDL_RenderFillRect(renderer, &enemyRect);

        // Update the position of the text based on the position of the enemy
        enemyTexts[i].object.x = enemies[i].x;
        enemyTexts[i].object.y = enemies[i].y - enemyTexts[i].object.height;

        // Render hit count text
        renderText(&enemyTexts[i]);
    }

    // Render bullets
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
    for (int i = 0; i < 10; ++i) {
        if (bullets[i].object.y > 0 && !bullets[i].hit) {
            SDL_Rect bulletRect = {bullets[i].object.x, bullets[i].object.y, bullets[i].object.width, bullets[i].object.height};
            SDL_RenderFillRect(renderer, &bulletRect);
        }
    }

    // Render quit text
    SDL_Color textColor = {255, 255, 255}; // White color
    SDL_Surface* quitTextSurface = TTF_RenderText_Solid(font, quitText.message.c_str(), textColor);
    SDL_Texture* quitTextTexture = SDL_CreateTextureFromSurface(renderer, quitTextSurface);
    SDL_Rect quitTextRect = {quitText.object.x, quitText.object.y, quitText.object.width, quitText.object.height};
    SDL_RenderCopy(renderer, quitTextTexture, NULL, &quitTextRect);

    SDL_FreeSurface(quitTextSurface);
    SDL_DestroyTexture(quitTextTexture);

    // Update the screen
    SDL_RenderPresent(renderer);
}

void updateGame() {
    // Update enemies (example: move them down the screen)
    for (int i = 0; i < 3; ++i) {
        enemies[i].y += 1;
    }

    // Update bullets (example: move them up the screen)
    for (int i = 0; i < 10; ++i) {
        if (bullets[i].object.y > 0 && !bullets[i].hit) {
            bullets[i].object.y -= 5;

            // Check for bullet-enemy collision (example)
            for (int j = 0; j < 3; ++j) {
                if (bullets[i].object.x < enemies[j].x + enemies[j].width &&
                    bullets[i].object.x + bullets[i].object.width > enemies[j].x &&
                    bullets[i].object.y < enemies[j].y + enemies[j].height &&
                    bullets[i].object.y + bullets[i].object.height > enemies[j].y) {
                    // Bullet hit enemy
                    bullets[i].hit = true; // Set the hit flag for this bullet
                    enemyTexts[j].value -= 1; // Update hit count text

                    // Check if the enemy ship is destroyed
                    if (enemyTexts[j].value <= 0) {
                        // Add additional logic to handle the destruction of the enemy ship
                        // For now, let's respawn the enemy ship with a new hit count
                        enemies[j].y = 50;
                        enemyTexts[j].value = 3; // Reset hit count
                    }
                    // You can add more logic here for scoring, etc.
                }
            }
        }
    }
}

void handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            closeSDL();  // Close SDL if the user clicks the close button
        } else if (e.type == SDL_KEYDOWN) {
            // Handle keydown events to allow continuous movement
            if (e.key.keysym.sym == SDLK_LEFT) {
                player.x -= 10;
            } else if (e.key.keysym.sym == SDLK_RIGHT) {
                player.x += 10;
            } else if (e.key.keysym.sym == SDLK_SPACE) {
                // Find an available bullet slot and set its position
                for (int i = 0; i < 10; ++i) {
                    if (bullets[i].object.y <= 0) {
                        bullets[i].object.x = player.x + (player.width - bullets[i].object.width) / 2;
                        bullets[i].object.y = player.y;
                        bullets[i].hit = false; // Reset the hit flag
                        break;
                    }
                }
            } else if (e.key.keysym.sym == SDLK_q) {
                closeSDL();  // Close SDL if Q is pressed
            }
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

