/* DRAW.C contains graphics screen routines */

#include <stdlib.h>
#include <dos.h>
#include <conio.h>

#include "tankdefs.h"
#include "types.h"
#include "extern.h"

#define F      25 /* border foreground color */
#define BCOLOR 25 /* color of blockades on screen */
#define TNAME  0x07 /* tank name color */
#define SC   0x0F /* score color */
#define TC   0x0E /* SUPER TANK */

void drawit(int x, int y, int c, int r, char *buf)
/* requires: x,y coordinates of upper left corner object
 * 			 c,r dimensions of object
 * 			 buf = array of char colors
 * ensures:  object drawn on screen
 */
{  register int r1,c1;
	for(r1=0;r1<r;r1++)
		for(c1=0;c1<c;c1++) *(vbuf+(y+r1)*320+x+c1)=*(buf+r1*c+c1);
}

/* DRAWBOX: Draws boxes on the screen
 * requires: vid mode 13
 * ensures:  box drawn with corner coords as given
 */
void drawbox(int xl, int yl, int xr, int yr, char c)
{
	int x,y;
	for(y=yl;y<yr;y++)
	  for(x=xl;x<xr;x++) *(vbuf+y*320+x)=c;
}


void restart()
/*	requires: none
 * ensures:  graphics mode set, screen drawn, varaibles reset as req'd.
 */
{  int x,y,x1,y1,i;
	char temp[20];
	tankRec *t;

	mode(0x13);

	PutString13( "                                        ",0,0,F,0);
	PutString13( "ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",0,8,F,0);
	PutString13( "³                                      ³",0,16,F,0);
	PutString13( "³                                      ³",0,24,F,0);
	PutString13( "³                                      ³",0,32,F,0);
	PutString13( "³                                      ³",0,40,F,0);
	PutString13( "³                                      ³",0,48,F,0);
	PutString13( "³                                      ³",0,56,F,0);
	PutString13( "³                                      ³",0,64,F,0);
	PutString13( "³                                      ³",0,72,F,0);
	PutString13( "³                                      ³",0,80,F,0);
	PutString13( "³                                      ³",0,88,F,0);
	PutString13( "³                                      ³",0,96,F,0);
	PutString13( "³                                      ³",0,104,F,0);
	PutString13( "³                                      ³",0,112,F,0);
	PutString13( "³                                      ³",0,120,F,0);
	PutString13( "³                                      ³",0,128,F,0);
	PutString13( "³                                      ³",0,136,F,0);
	PutString13( "³                                      ³",0,144,F,0);
	PutString13( "³                                      ³",0,152,F,0);
	PutString13( "³                                      ³",0,160,F,0);
	PutString13( "³                                      ³",0,168,F,0);
	PutString13( "³                                      ³",0,176,F,0);
	PutString13( "ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ",0,184,F,0);

	if (NumTanks==3) {
		PutString13( "Yellow:", 120, 0, TNAME, 0);
	   sprintf(temp,"%-2d",tank[2].score);
	   PutString13(temp, 176, 0 ,SC,0);
	} else PutString13( "SUPER TANK",112,0,TC,0);
	PutString13( "Red:",48,0,TNAME,0);
	PutString13( "Blue:",220,0,TNAME,0);
	sprintf(temp,"%-2d",tank[1].score);
	PutString13(temp,80,0,SC,0);
	sprintf(temp,"%-2d",tank[0].score);
	PutString13(temp,259,0,SC,0);

	newscreen=0;

  	/* reset the field array */
  	for(x=0;x<FIELDX;x++)
		for(y=0;y<FIELDY;y++) field[x][y]=255;

  	/* draw some random blocks */
	for (i=0;i<23;i++) {
		x = 30 + 20 * random(13);
		y = 30 + 20 * random(7);
		drawbox(x, y, x+20, y+20, BCOLOR);
	}

	/* draw trees if option selected */
	if (TreesOn)  {
		for (i=0;i<NumTanks;i++) tank[i].treeshit=0;
		for (i=0;i<NUMTREES;i++) {
			x = 30 + 20 * random(13);
			y = 44 + 20 * random(6);
			/* enter each non-zero position in the bitmap into field[][] */
			for (y1=0;y1<TREEYDIM;y1++) {
				for(x1=0;x1<TREEXDIM;x1++)
					if ( *(TreeBM+y1* TREEXDIM +x1)!=0) field[(x1+x)][(y1+y)]=i;
			}
			tree[i].x=x;
			tree[i].y=y;
			drawit(x, y, TREEXDIM, TREEYDIM, TreeBM);
		}
 	}

	/* initialize each tank */
	for (i=0;i<NumTanks;i++) {
		tank[i].speed=tank[1].speed=TankSpeed;
		tank[i].x=TankInitialX[i];
		tank[i].y=TankInitialY[i];
		drawit(tank[i].x, tank[i].y, TXDIM, TYDIM, tank[i].pic[tank[i].dir]);
		shotdata[i].range=0; /* reset all shots */
	}

}