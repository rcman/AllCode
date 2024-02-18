#include "main.h"
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

void check_input()

{
       // int c;
       // apply_surface( 200.200,  man, screen, &enemy1[0] );



        if (motionButtonDown[MOTIONBUTTON_LEFT])
        {
        player->x -=player->velx;

        }


        if (motionButtonDown[MOTIONBUTTON_RIGHT])
        {

        player->x +=player->velx;

        }

        if (motionButtonDown[MOTIONBUTTON_UP])
        {

        player->y -=player->vely;

        }

        if (motionButtonDown[MOTIONBUTTON_DOWN])
        {

        player->y +=player->vely;

        }

      if (actionButtonDown[ACTIONBUTTON_A])
	{
		if (++player->firingcounter >= player->firingdelay)
		{
			// first we need to save the ships x and Y so we know where the bullet starts from
			// save the bullet in the bullet array and turn it on
			// cycle through the array and move the bullets that are on

			for (int i = 0;i < MAX_SHOTS; i++)
			{
				if (!savedshots[i].shotalive)
				{
					// Save the Shot Record what we need to
					savedshots[i].shotalive = true;

					if (++player->shotsfired >= MAX_SHOTS)
					{
						player->shotsfired = 0;
					}

					if (player->shotsfired % 2)
					{
						savedshots[i].x = player->x;
					}
					else
					{
						savedshots[i].x = player->x + manA[0].w - manA[1].w;
					}
					savedshots[i].y = player->y + manA[0].h / 2;

					break;
				}
			}
			player->firingcounter = 0;
		}
	}


}
