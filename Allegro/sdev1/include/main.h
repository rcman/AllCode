// Main.h

// this makes sure that this file is never included multiple times
// and is something that is VERY important!
#ifndef __MAIN_H__
#define __MAIN_H__

// constants
const int SCREEN_WIDTH 	= 1024;
const int SCREEN_HEIGHT = 768;
const int SCREEN_BPP 	= 32;

const int MAPWIDTH 		= 32;
const int MAPHEIGHT 	= 24;

const int TILEHEIGHT 	= 32;
const int TILEWIDTH 	= 32;

struct MyPlayer{
		int x, y;			//x, y coordinate (top left of the player rectangle)
		int h, w;			//height, width
		int velx, vely;		//velocity on x, y axis
		bool faceright;		//player facing right? -> graphics
		bool lockjump;		//may the player jump
		bool jumping;
};


  // forward declare the SDL stuff that we need to create pointers to
struct SDL_Surface;
struct SDL_Rect;
union SDL_Event;


extern SDL_Surface*		screen;
//tileset = load_image("bmp/Sewers.bmp", 255, 0, 255);
extern SDL_Surface*		tileset;
//man
extern SDL_Surface*		man;
extern SDL_Rect*        manA;

extern SDL_Event* 		event;
extern int Map[];

extern MyPlayer* player;

extern bool init_player();
extern void destroy_player();
extern bool Gameinit();
extern bool load_files();
extern bool load_man();
extern SDL_Surface* load_image(const char* filename, unsigned char r, unsigned char g, unsigned char b);
extern void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = 0);
extern void InitMap();
extern void RenderMap();
void moveplayer();
// we MUST close the #ifndef from above
#endif


