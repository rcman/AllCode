#include "main.h"
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

bool init_player()
{
        if (!player) {player = new MyPlayer; return true;}
        return false;
}
