// Defender style game like Datastorm
// Date Start: April 23, 1995
// Revision: May 7, 1995
// Single Bottom Screen Scroll
// Task List    Init Screen
//              Load Objects
//              Init Enemies
//              Calculate Coords
//              Move Enemies
//              Scroll Screen
//**********************************************************
//    PUT (x, SIN(ya * (3.14159267# / 180)) * 60 + 100), Box%, XOR
//        LOCATE 1, 1: PRINT SIN(ya); ya; yac
//        yao = ya
//        xo = x
//        PUT (x, SIN(ya * (3.14159267# / 180)) * 60 + 100), Box%, XOR
//        ya = ya + yac
//        IF ya > 360 THEN ya = 0: ' ya = 360
//        'IF ya < 0 THEN yac = 1: ya = 0
//        x = x + .3
//        FOR t = 1 TO 10: NEXT t
//        PUT (xo, SIN(yao * (3.14159267# / 180)) * 60 + 100), Box%, XOR
//        GOTO mainloop
//

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <math.h>
// #include <complex.h>

//#include <conio.h>
//#include <graphics.h>
#pragma inline
void Init_Mode(void);
void Close_Mode(void);
void CutImage(int saveareax, int savesreay, int spritenum);
void PutImage(int spx,int spy);
void RestoreBack(int spx,int spy, int spritenum );
void getch(void);
void initarray(void);
void copypage(char *source, char *destin);
int kbhit(void);
int getkey(void);

//Global

char far *screen = MK_FP(0xa000,0);
char far *destin;
					// 16 x 16 array

void *buffer;
void *image;

unsigned int imsize;
int top = 290;
int bottom = 35;
int rbottom = 300;
int lbottom = 145;
int spx=100;            // Pixels
int spy=100;
int spritenum=0;
int lp;
int x,y,xy1,xy,fl,tm;
int z=0;
int offset = 0;
int numspr=30;
int sprloc=16;
int stars[30][30];

void initarray(void)
{


	int c,sn,tempx,tempy;
	randomize();

	for (sn=0;sn<=3;sn++)
	{
		for (c=0 ; c<numspr; c++)
		{
		tempx=random(319);
		tempy=random(169);
		starsx[c]=tempx;
		starsy[c]=tempy;
		starsspeed[c]=random(6)+1;

		PutImage(tempx, tempy);
		}
  }

}


void main(void)
{
	int i,direction=1,xx,swav;

	Init_Mode();
	initarray();
	do {



	 } while (!kbhit());



	Close_Mode();
}


void PutImage(int spx,int spy)
{
		  offset = (spy * 320) + spx;


		  *(screen+offset)^=1;

}




void Init_Mode(void)
{
	asm {
		mov ax,0x13
		int 0x10
		 }
}

void Close_Mode()
{

	asm {
		mov ax,0x03
		int 0x10
		}

}


