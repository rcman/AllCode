
//The headers
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>
#include "main.h"

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

void clean_up()
{
    // free the memory we allocated in Gameinit()

    if (tileset) { SDL_FreeSurface(tileset); }

    // event is not an array, so we do not use the array-form of the delete operator:
    if (event) { delete event; }
    // Delete Player


}

int main( int argc, char* args[] )
{
    //Quit flag
    bool quit = false;

    //Initialize
    if (!Gameinit())
    {
        return 1;
    }

    //While the user hasn't quit
    while(!quit)
    {
        //While there's events to handle
        while( SDL_PollEvent( event ) )
        {
            //If the user has Xed out the window
            if( event->type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }

            // the user hits ESC
            if (event->type==SDL_KEYDOWN &&(event->key.keysym.sym==SDLK_ESCAPE)){quit=true;}
        }

        SDL_Flip(screen);
        SDL_Delay(20);
    }

    //Free the images and quit SDL
    clean_up();

    return 0;
}
