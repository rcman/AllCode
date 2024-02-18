
#include <SDL/SDL.h>
#include "main.h"

SDL_Surface* screen = 0;

SDL_Surface* tileset = 0;
SDL_Surface* man = 0;
SDL_Event* event =0;
MyPlayer* player = 0;
SDL_Rect* manA;
//The event structure

bool Gameinit()
{
    // initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }
    atexit(SDL_Quit);

    // set up the screen
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF);

    // if there was an error in setting up the screen
    if (!screen)
    {
        return false;
    }

    if (!init_player()){ return false; };


    //Set the window caption
    SDL_WM_SetCaption( "Franco Gaetan's Game Demo - Cleaned by Richard Marks", 0 );

    // create the sdl event
    event = new SDL_Event;


    // load the files
    if (!load_files())
    {
        return false;
    }

    // load man files
    if (!load_man())
    {
        return false;
    }

    manA = new SDL_Rect[8];

    manA[0].x = 0;
    manA[0].y = 0;
    manA[0].w = 32;
    manA[0].h = 32;

	// clear screen to black
	SDL_FillRect(screen, 0, 0);

	// draw the map
	SDL_Rect src, dst;
	for (int my = 0; my < MAPHEIGHT; my++)
	{
		dst.y = my * TILEHEIGHT;
		for (int mx = 0; mx < MAPWIDTH; mx++)
		{
			src.x = ((Map[mx + (my * MAPWIDTH)]) % 10) * TILEWIDTH;
			src.y = ((Map[mx + (my * MAPWIDTH)]) / 10) * TILEHEIGHT;
			src.w = src.h = TILEWIDTH;
			dst.x = mx * TILEWIDTH;

			SDL_BlitSurface(tileset, &src, screen, &dst);
		}
	}

    apply_surface( 10, 10, man, screen, &manA[0]);
	return true;
}

