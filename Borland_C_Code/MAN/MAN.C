//Code By Franco Gaetan
//E-Mail franco@inforamp.net
//http://www.inforamp.net/~franco
//


#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mem.h>
#include "draw.h"

#define TOTALSHAPE 110
#define keypressed (bioskey(1) != 0)
#define kb_clear() while keypressed bioskey(0);

void CutImage(int spx,int spy);
void PutImage(int spx,int spy, int spn);
void RestoreImage(int spx,int spy);
void getch(void);
void initarray(void);
int kbhit(void);
void initscreen(void);
int fileread(void);
void SyncToVerticalRetrace( void );
void GotoXY(int x,int y);
void disp_stick(int line,struct joy1 *joy);
int choose_stick(struct joy1 *joy);
void read_stick(int stick,struct joy1 *joy);


struct joy1 {
	 int sw1,sw2;
	 int x,y;
	 int cenx,ceny;
	 } joystick;

//joy_stick joy;

struct fshape
	{

		int w,h;
		int n,c;
		int flag;
		int rowflag;
		char far shp[260];
					// TOTALSHAPE  =  20
	} fshp[TOTALSHAPE];

struct save
	{
		char savearea[260];
	} saved[10];


void *image;

int k;
int spx=100;            // Pixels
int spy=100;
int lp;
int x,y,xy1,xy,fl,tm;
int z=0;
unsigned int offset = 0;
int numspr=100;

char far *screen  = MK_FP(0xa000,0);
char far *destin;

FILE *in, *out, *outdata;

// Define Walk Numbers
//int walkL[4]={0,1,2,3};
//int walkR[4]={4,5,6,7};




void initscreen(void)
{

	destin = (char far *) farmalloc(64000l);

	if (destin == NULL)
		{
			 printf("Not enough memory to allocate screen buffer\n");
			 exit(1);  /* terminate program if out of memory */
		}
		_fmemset(destin, 0, 64000);
}


void main(void)
{
	int i,xx,swav;
	xx=300;

//	k = choose_stick(&joystick);
  //	ClrScr();

	Init_Mode();
	fileread();
	PutImage(xx,100,0);
	do{

	read_stick(k,&joystick);

	disp_stick(0,&joystick);

	if (joystick.x<-330)
	{

	for (i=0;i<4;i++)
	{
			CutImage(xx,100);
			PutImage(xx,100,i);
			delay(25);
			SyncToVerticalRetrace();
			RestoreImage(xx,100);
			xx =xx - 3;
			delay(25);

	 }
	 }
	 }while(xx>10);

	Close_Mode();

}




int fileread(void)
{

	int s=0,i;
	struct fshape *p;

	if ((in = fopen("man.std", "rb"))
		 == NULL)
	{
		fprintf(stderr, "Cannot open input file.\n");
		return 1;

	}

	i=0;
	for (s=0,p=&fshp[s];s<TOTALSHAPE && feof ;s++,p=&fshp[s],i=0)

	while (i<256)
		{
	*(p->shp+i)=fgetc(in);
  //      printf("%x",*(p->shp+i));
	i++;
		}

	fclose(out);

	return 1;

}


void PutImage(int spx,int spy, int spn)
{
	offset=spy*320+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
		  *(screen+y+offset)^=fshp[spn].shp[x*16+y];
		}
		offset = offset + 320;      //bytes to next line
	}


}
void CutImage(int spx,int spy)
{
	offset=spy*320+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
		  saved[1].savearea[x*16+y]=*(screen+y+offset);
		}
		offset = offset + 320;      //bytes to next line
	}

}

void RestoreImage(int spx,int spy)
{
	offset=spy*320+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
		  *(screen+y+offset)=saved[1].savearea[x*16+y];
		}
		offset = offset + 320;      //bytes to next line
	}

}


void SyncToVerticalRetrace( void )
  {
  // If we happen to be in the middle of a vertical retrace
  // period wait until its over.

  while( inp( 0x3da ) & 8 )
	 ;

  // Wait until we begin vertical retrace.

  while( !(inp( 0x3da ) & 8) )
	 ;
  }

void GotoXY(int x,int y)
{
	 union REGS r;
/* Set XY position */
	 r.h.ah = 2;
	 r.h.bh = 0;            /* Assume Video Page 0 */
	 r.h.dh = (char) y;
	 r.h.dl = (char) x;
	 int86(16,&r,&r);
}


void disp_stick(int line,struct joy1 *joy)
{
	 GotoXY(0,line);
	 printf("sw1 %d sw2 %d",joy -> sw1,joy -> sw2);
	 GotoXY(0,line+1);
	 printf("x %4d y %4d",joy -> x,joy -> y);
}

void read_stick(int stick,struct joy1 *joy)
{
	 int k,jx,jy;
	 int c,m1,m2,m3,m4,m5;

	 if (stick == 1) m4 = 1; else
	 if (stick == 2) m4 = 4; else
	 printf("Invalid stick %d\n",stick);

	 m5 = m4 << 1;
	 m1 = m4 << 4;
	 m2 = m5 << 4;
	 m3 = m4 + m5;

/* Trigger joystick */
	 outportb(0x201,0xff);
	 c = inportb(0x201);

/* Read switch settings */
	 joy -> sw1 = (c & m1) == 0;
	 joy -> sw2 = (c & m2) == 0;

/* Get X and Y positions */
	 for (k = 0; (c & m3) != 0; k++) {
	if ((c & m4) != 0) jx = k;
	if ((c & m5) != 0) jy = k;
	c = inportb(0x201);
	 }
	 joy -> x = jx - (joy -> cenx);
	 joy -> y = jy - (joy -> ceny);
}

int choose_stick(struct joy1 *joy)
{
	 int init_swa,init_swb,swa,swb;
	 int c,retval;

	 printf("Center joystick and press fire, or press any key\n");
	 kb_clear();
	 outportb(0x201,0xff);
	 c = inportb(0x201);
	 init_swa = c & 0x30;
	 init_swb = c & 0xc0;
	 do {
		 outportb(0x201,0xff);
		 c = inportb(0x201);
		 swa = c & 0x30;
		 swb = c & 0xc0;
	 } while ((swa == init_swa) && (swb == init_swb) && !keypressed);
	 if (swa != init_swa) {
		 printf("Joystick 1 selected\n");
		 retval = 1;
	 } else if (swb != init_swb) {
		 printf("Joystick 2 selected\n");
		 retval = 2;
	 } else {
		 printf("Keyboard selected\n");
		 kb_clear();
		 retval = 0;
	 }

	 if (retval != 0) { /* Determine Center */
		 joy -> cenx = joy -> ceny = 0;
		 read_stick(retval,joy);
		 joy -> cenx = joy -> x;
		 joy -> ceny = joy -> y;
	 }

	 return(retval);
}


