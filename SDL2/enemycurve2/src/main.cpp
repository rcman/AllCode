#include <SDL2/SDL.h>
#include <math.h>
#include <stdlib.h>
#define NUM_ENEMIES 25

int main(int argc, char* argv[]) {
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Enemy Movement", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Set the enemies starting position
    int x[NUM_ENEMIES];
    int y[NUM_ENEMIES];

    for (int i = 0; i < NUM_ENEMIES; i++) {
        x[i] = 320;
        y[i] = 0;
    }

    // Set the angle of movement (in radians)
    double angle = 0;
    double angle_inc = 0.05;

    // Set the amplitude of the curved path
    double amplitude = 10;
    // Set the frequency of the curved path
    double frequency = 0.01;

    // Main loop
    while (1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                goto end;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < NUM_ENEMIES; i++) {
            // Update the enemies' position
            y[i] += sin(angle) * amplitude * sin(frequency * x[i]);
            x[i] += cos(angle) * amplitude * cos(frequency * y[i]);

            // Check if the enemy goes off the screen
            if (x[i] < 0) {
                x[i] = rand() % 640;
            } else if (x[i] > 640) {
                x[i] = rand() % 640;
            }
            if (y[i] < 0) {
                y[i] = rand() % 480;
            } else if (y[i] > 480) {
                y[i] = rand() % 480;
            }

            // Draw the enemy
            SDL_Rect rect = {x[i], y[i], 10, 10};
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
        // Increment the angle
        angle += angle_inc;

        // Render the screen
        SDL_RenderPresent(renderer);

        // Wait for a bit
        SDL_Delay(10);
    }

end:
    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

