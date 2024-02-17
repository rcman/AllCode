// By: Franco Gaetan
// Date Start: Dec 2, 1996

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <bios.h>
#include <conio.h>
#include "draw.h"

#define TOTALSHAPE 100
#define TOTALANIMS 10
#define TOTALOBJECTS 20

void StartShot(void);
void checkkeys(void);
void moveshots(void);
int  getch(void);
int getk(void);
void CutImage(int spx,int spy);
void PutImage(int spx,int spy);
void RestoreBack(int spx,int spy);
int fileread(void);
void SyncToVerticalRetrace( void );
void moveship(int x, int y);


//Global

char far *screen = MK_FP(0xa000,0);
char far *destin;

int ext=0;
int hc,lc,rc;
int shotnumber=0;

int spx=100;            // Pixels
int spy=100;
int x,y,xy1,xy,fl,tm,s,t,l;
unsigned int offset = 0;

//////////////////////////

char *pc[20];
unsigned char key;

int leng=0;
int totalleng=0;
int spaceleftb=0;
int spaceleftt=0;
int bottom=190;	// bottom let add X to it to get shape
int line=0;
int let=0;
char p;
int v=0;


int nline=0;
int pixelcount=0;
int linecount=0;
int spritenum=0;



FILE *in, *out, *outdata, *indata, *std, *map, *mapin;


struct fshape
	{

		int w,h;
		int n,c;
		int flag;
		int rowflag;
		char far shp[260];
					// TOTALSHAPE  =  20
	} fshp[TOTALSHAPE];

struct shots
	{
		int x,y;
		int flag;
	} totalshots[1000];


struct animshape {
		int active;
		int animwidth;
		int animheight;
		int animox;
		int animoy;
		int animx;
		int animy;
		int prox;
		int animspeed;
		int currentshape;
		int oldshape;
		int max;
		int row;

		struct fshape *fshp[TOTALSHAPE];        // 10 Pointers to data

	 } animobjects[TOTALANIMS];


void main()
{

	int c;
	Init_Mode();
	fileread();				// draw first line
	PutImage(100,100);
	do{

	 int getk();
	 moveship(x,y);

		}while(x>1);


	getch();
	Close_Mode();
}


int fileread(void)
{

	int s=0,i;
	struct fshape *p;

	if ((in = fopen("defend.std", "rb"))
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

	fclose(in);

	return 1;

}

void CutImage(int spx,int spy)
{
	int i=0;
	offset=spy*320+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
				fshp[2].shp[x*16+y]=*(screen+y+offset);

		}
		offset = offset + 320;      //bytes to next line
	}


}

void PutImage(int spx,int spy)
{
	offset=spy*320+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
		  *(screen+y+offset)^=fshp[30].shp[x*16+y];
		}
		offset = offset + 320;      //bytes to next line
	}

}

void RestoreBack(int spx,int spy)
{
	int i=0;
	offset=spy*320+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
			 *(screen+y+offset)=fshp[2].shp[x*16+y];
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




void StartShot(void)
{

//	printf("h");


	totalshots[shotnumber].x=100;
  //	totalshots[shotnumber].y=spy;
  //	spy = spy - 1;
	totalshots[shotnumber].y=190;
	shotnumber = shotnumber + 1;				// increment the shot count


}

void moveshots(void)
{
	int i;

	for (i=0;i<totalshots;i++)
		{
			spx = totalshots[i].x;
			spy = totalshots[i].y;
		 //	printf("spx= %d  spy = %d",spx,spy);
		 //	getch();

		  //	CutImage(spx,spy);
		 //	delay(10);
			PutImage(spx,spy);
			 //					delay(10);
		  //	RestoreBack(spx,spy);

			spy = totalshots[i].y;
			spy = spy - 1;
			if (spy <10)
				{
				spy=190;
				shotnumber=shotnumber-1;
				}


			totalshots[i].y=spy;
			getch();
		}


}
/*

//' Subroutine to fire shot from your ship
//--------------------------------- SHOT.BAS -------------------------------¦+-+
//'move shot across the screen                                                  
//moveshots:                                                                    _
//		  FOR sh = 1 TO 40                                                      _
//		  IF sht(1, 1, sh) = 0 THEN GOTO sap                                    _
//		  nx = sht(1, 1, sh)                                                    _
//		  ny = sht(1, 2, sh)                                                    _
//		  IF nx > 310 OR nx < 10 THEN sht(1, 1, sh) = 0: PUT (nx, ny + 2), Box%,_
//		  PUT (nx, ny + 2), Box%, XOR                                           _
//		  nx = nx + sht(1, 3, sh)                                               _
//		  sht(1, 1, sh) = nx                                                    _
//		  PUT (nx, ny + 2), Box%                                                _
//sap:                                                                          _
//		  NEXT sh                                                               _
//		  RETURN
																										_

//fireshot:
//		  FOR i = 1 TO 40
//		  IF sht(1, 1, i) = 0 THEN GOTO ov
//		  NEXT i
//		  RETURN
//ov:
//		  sht(1, 1, i) = x
 //		  sht(1, 2, i) = sy: PUT (x, sy + 2), Box%
 //		  sht(1, 3, i) = smv
//
//

*/

int getk(void)
{
	asm {
		mov ah,0
		in al,0x60
		mov [key],al
		 }
			 if (key==75)
			 {
			 spx = totalshots[shotnumber].x;
			 spx = spx -1;
			 }

			 if (key==77)
			 {
			 spx = totalshots[shotnumber].x;
			 spx = spx + 1;
			 }
			 if (key==72)
		{
		StartShot();
		}
	 return(key);
}


void moveship(int x, int y)
{

         PutImage(spx,spy);


}
