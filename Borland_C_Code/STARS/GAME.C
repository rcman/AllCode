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


