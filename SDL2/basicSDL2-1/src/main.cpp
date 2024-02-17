#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main(int argc, char* argv[]) {
    // Initialize SDL and create a window and renderer
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // Load the image and create a texture from it
    SDL_Surface* surface = SDL_LoadBMP("image.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Define the source and destination rectangles
    SDL_Rect srcrect = { 0, 0, 32, 32 };
    SDL_Rect dstrect = { 100, 100, 64, 64 };

    // Draw the image to the screen
    SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    SDL_RenderPresent(renderer);

    // Wait for a key press and clean up
    SDL_Event event;
    while (SDL_WaitEvent(&event) && event.type != SDL_QUIT && event.type != SDL_KEYDOWN) {}
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

