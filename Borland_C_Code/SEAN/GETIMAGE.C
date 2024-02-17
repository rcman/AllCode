#include <graphics.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "evntmngr.h"

int event_mask=JOYSTICK_EVENTS;
int ranvalx,ranvaly,i,xx,yy,j,oldx,oldy;
int dir =1;
int top = 290;
int bottom = 135;
int rbottom = 300;
int lbottom = 145;
int numimages = 10;
int xarray[40];
int yarray[40];
int direct[40];
int speed=5;
void *image;
unsigned int imsize;

void endpgm(void);
void initdirection(void);
void drawsquare(void);
void initgraphics(void);
void mvobjects(void);
void startobjects(void);
void gravity(void);
void main(void);
int getourevent(void);

void endpgm(void)
{
settextjustify (CENTER_TEXT,BOTTOM_TEXT);
outtextxy (320,320,"Press any key to exit");
getch();

closegraph();


}



void initdirection(void)
{

	for (j=0;j<numimages;j++) {              // Set Speed at Variable
		direct[j]=speed;                // Stupid Eh?
		}

	for (i=0;i<numimages;i++) {             // randomize positions
	      ranvalx = random(600);
	      ranvaly = random(439);
	      xarray[i]=ranvalx;
	      yarray[i]=ranvaly;
//	      putimage (ranvalx,ranvaly,image,COPY_PUT);
	      }

}



void drawsquare(void)
{

int gdriver = EGA;
int gmode = EGAHI;

	/* set EGA high resolution 16 color mode */

initgraph (&gdriver,&gmode,"");
rectangle (0,0,639,349);

	// draw and store an image - then place it
	// in the four corners of the screen

setfillstyle (SOLID_FILL,2);
bar (top,bottom,rbottom,lbottom);
setfillstyle (SOLID_FILL,4);
imsize = imagesize (top,bottom,rbottom,lbottom);
image = malloc(imsize);
getimage (top,bottom,rbottom,lbottom,image);
putimage (top,bottom,image,XOR_PUT);

}


void initgraphics(void)
{
	// register EGAVGA_driver and sanserif_font
	// these have been added to the graphics.lib
	// as described in util.DOC

registerbgidriver (EGAVGA_driver);
registerbgifont (sansserif_font);


}


void mvobjects(void)
{

	for (yy=0;yy<numimages;yy++) {
	      ranvalx=xarray[yy];
	      ranvaly=yarray[yy];
		      if (ranvalx <1)
			{
				direct[yy]=speed;
			      //	sound(950);
			      //	delay(5);
			      //	nosound();
			}
		      if (ranvalx >639)
			{
				direct[yy]=-speed;
			     //	sound(950);
			     //	delay(5);
			     //	nosound();
			}
	       oldx=ranvalx;
	       ranvalx = ranvalx + direct[yy];
	       xarray[yy]=ranvalx;

	       putimage (ranvalx,ranvaly,image,XOR_PUT);
	       putimage (oldx,ranvaly,image,XOR_PUT);
	       }

}

void gravity(void)
{


}


void startobjects(void)
{

	for (yy=0;yy<numimages;yy++) {
	      ranvalx=xarray[yy];
	      ranvaly=yarray[yy];
	      putimage (ranvalx,ranvaly,image,XOR_PUT);
      }

}

void main(void)
{
	init_events();
	printf("Center Joystick\n");
	setcenter();
	getch();
	printf("Upper Left\n");
	setmin();
	getch();
	printf("Lower Right\n");
	setmax();
        getch();



	initgraphics();
	randomize();
	initdirection();
	drawsquare();
	startobjects();

		for (xx=0;xx<1400;xx++) {      // Move objects for 1400 Tics
		getourevent();
		mvobjects();
//      	getch();

	}
	endpgm();                       // delay and exit

	// return 0;
}


int getourevent ()
{
	int xcoor;
	int ycoor;

	xcoor = readstick(1);   // get X-coor
	ycoor = readstick(2);   // get Y-coor
	if (xcoor<100) printf("left  ");
	if (xcoor>700) printf("right ");
	if (ycoor<100) printf("up\n");
	if (ycoor>300) printf("down\n");
}