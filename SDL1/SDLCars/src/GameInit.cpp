
#include <SDL/SDL.h>
#include "main.h"

SDL_Surface* screen = 0;
SDL_Surface* tileset = 0;

SDL_Surface* man = 0;
SDL_Rect* manA;

SDL_Surface* enemy = 0;
SDL_Rect* enemy1;

SDL_Surface* bg = 0;
SDL_Rect* backgnd;


MyPlayer* player = 0;
Shipshots* shots = 0;

SDL_Event* 	event = 0;

bool motionButtonDown[4] = {false, false, false, false};

Shipshots savedshots[MAX_SHOTS];
Enemyspots savedenemy[MAX_ENEMIES];


bool actionButtonDown[1];

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
    SDL_WM_SetCaption( "Franco Gaetan's Shooter Game Demo ", 0 );

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

    // load man files
       if (!load_bg())
    {
        return false;
    }



    int i;



    for (i=0;i<MAX_SHOTS;i++)
    {
        savedshots[i].shotalive = false;
        savedshots[i].x = 0;
        savedshots[i].y = 0;
        savedshots[i].h = 32;
        savedshots[i].w = 32;
        savedshots[i].velx = 0;
        savedshots[i].vely = 0;

    }

    for (i=0;i<MAX_ENEMIES;i++)
    {
        //savedenemy[i].x = 10 + rand() % (500 - 10 );
        //savedenemy[i].y = 10 + rand() % (500 - 10 );
        savedenemy[i].x = 210;
        savedenemy[i].y = 0;


        savedenemy[i].h = 32;
        savedenemy[i].w = 32;
        savedenemy[i].velx = 8;
        savedenemy[i].vely = 8;
        savedenemy[i].enemyalive = false;
        savedenemy[i].collisiondetected = false;
        savedenemy[i].animnumber = 0;



};



    // plane is 704 and 109
    // 32 * 4 on Y = 128
    // 24 * 32 = 768

    manA = new SDL_Rect[4];            // Main player needs to be changed
    enemy1 = new SDL_Rect[4];          // Enemy array
    backgnd = new SDL_Rect[2];

    player->animnumber = 0;
    player->y = 650;
    player->x = 599;
    player->velx = 8;
    player->vely = 8;
    player->firingdelay =0;
    player->firingcounter = 0;
    player->shotsfired = 0;

    savedenemy[0].enemyalive = true;

    manA[0].x = 0;
    manA[0].y = 0;
    manA[0].w = 32;
    manA[0].h = 32;

    manA[1].x = 0;
    manA[1].y = 0;
    manA[1].w = 32;
    manA[1].h = 32;

    backgnd[0].x = 0;
    backgnd[0].y = 0;
    backgnd[0].w = 1024;
    backgnd[0].h = 768;


    enemy1[0].x = 0;  // car
    enemy1[0].y = 0;
    enemy1[0].w = 32;
    enemy1[0].h = 32;

	// clear screen to black
	SDL_FillRect(screen, 0, 0);


	return true;
}

