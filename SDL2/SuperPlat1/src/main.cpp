#include <SDL2/SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void renderPlatforms(SDL_Renderer* renderer, SDL_Texture* platformTexture, SDL_Rect* platformRects, int count) {
    for (int i = 0; i < count; i++) {
        SDL_RenderCopy(renderer, platformTexture, NULL, &platformRects[i]);
    }
}

int main(int argc, char* argv[]) 
{
    	// Initialize SDL
    	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    	{
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    	}

    // Create window
    SDL_Window* window = SDL_CreateWindow("Platform Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) 
    {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Load player image
    SDL_Surface* playerSurface = SDL_LoadBMP("player.bmp");
    if (playerSurface == nullptr) 
    {
        std::cout << "Failed to load player image! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);
    if (playerTexture == nullptr) {
        std::cout << "Failed to create player texture! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Load platform image
    SDL_Surface* platformSurface = SDL_LoadBMP("platform.bmp");
    if (platformSurface == nullptr) 
    {
        std::cout << "Failed to load platform image! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Texture* platformTexture = SDL_CreateTextureFromSurface(renderer, platformSurface);
    SDL_FreeSurface(platformSurface);
    
    if (platformTexture == nullptr) 
    {
        std::cout << "Failed to create platform texture! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // Create an array of platform rectangles that will hold the coordinates of all the platform rectangles that are being rendered on the screen
    SDL_Rect platformRects[800/32*100/32];
    int platformCount = 0;
    for (int x = 0; x < 800; x += 32) {
        for (int y = 550; y < 600; y += 32) {
            platformRects[platformCount].x = x;
            platformRects[platformCount].y = y;
            platformRects[platformCount].w = 32;
            platformRects[platformCount].h = 32;
            platformCount++;
        }
    }

    // Load ladder image
    SDL_Surface* ladderSurface = SDL_LoadBMP("ladder.bmp");
    if (ladderSurface == nullptr) {
        std::cout << "Failed to load ladder image! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Texture* ladderTexture = SDL_CreateTextureFromSurface(renderer, ladderSurface);
    SDL_FreeSurface(ladderSurface);
    if (ladderTexture == nullptr) {
        std::cout << "Failed to create ladder texture! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create an array of ladder rectangles that will hold the coordinates of all the ladder's rectangles that are being rendered on the screen
   // Create an array of ladder rectangles that will go straight up
    SDL_Rect ladderRects[600/32];
    int ladderCount = 0;
    int ladderX = 300;
    int aboveGround = 20;
    for (int i = 0; i < 4; i++) {
        for (int y = 550 - aboveGround - i*32; y < 550 - aboveGround - i*32 + 32; y += 32) {
            ladderRects[ladderCount].x = ladderX;
            ladderRects[ladderCount].y = y;
            ladderRects[ladderCount].w = 32;
            ladderRects[ladderCount].h = 32;
            ladderCount++;
        }
    }

    // Initialize player rect
    SDL_Rect playerRect = { 0, 550, 32, 32 };
    int playerSpeed = 2;
    int gravity = 1;



    // Main game loop
    while (true) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                goto cleanup;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
                    goto cleanup;
                }
            }
        }

        // Update the player position
        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_LEFT]) {
            playerRect.x -= playerSpeed;
        }
        if (state[SDL_SCANCODE_RIGHT]) {
            playerRect.x += playerSpeed;
        }
        if (state[SDL_SCANCODE_UP]) {
            // Check for collision between player and ladder
            for(int i = 0; i < ladderCount; i++){
                if (SDL_HasIntersection(&playerRect, &ladderRects[i])) {
                    playerRect.y -= playerSpeed;
                    playerRect.x = ladderRects[i].x + ladderRects[i].w/2 - playerRect.w/2;
                    //playerRect.y = ladderRects[i].y + ladderRects[i].h/2 - playerRect.h/2;
                    break;
                }
            }
        }
        if (state[SDL_SCANCODE_DOWN]) {
            // Check for collision between player and ladder
            for(int i = 0; i < ladderCount; i++){
                if (SDL_HasIntersection(&playerRect, &ladderRects[i])) {
                    playerRect.y += playerSpeed;
                    break;
                }
            }
        }
        playerRect.y += gravity;
        
        // check if player is outside the screen and move it back inside
        if (playerRect.x < 0) {
            playerRect.x = 0;
        }
        if (playerRect.y < 0) {
            playerRect.y = 0;
        }
        if (playerRect.x + playerRect.w > 800) {
            playerRect.x = 800 - playerRect.w;
        }
        if (playerRect.y + playerRect.h > 600) {
            playerRect.y = 600 - playerRect.h;
        }

        // Check for collision between player and platform
        for (int i = 0; i < platformCount; i++) {
            if (SDL_HasIntersection(&playerRect, &platformRects[i])) {
                playerRect.y = platformRects[i].y - playerRect.h;
                break;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
        
        // Render ladder
        for(int i = 0; i < ladderCount; i++){
            SDL_RenderCopy(renderer, ladderTexture, NULL, &ladderRects[i]);
        }
        // Render player
        SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

        // Render platforms
        renderPlatforms(renderer, platformTexture, platformRects, platformCount);

        

        // Update screen
        SDL_RenderPresent(renderer);
        SDL_Delay(15);
    }

cleanup:
    // Clean up
    SDL_DestroyTexture(ladderTexture);
    SDL_DestroyTexture(platformTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}



