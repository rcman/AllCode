// Star Simulator
// Date: June 24 - 1996
// By: Franco Gaetan
// Lots of Extra Code here eh?


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
void PutImage(int spx,int spy, int spritenum );
void RestoreBack(int spx,int spy, int spritenum );
void getch(void);
void initarray(void);
void copypage(char *source, char *destin);
int kbhit(void);
void SyncToVerticalRetrace( void );
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
int numspr=40;
int sprloc=16;

struct ship {
		int shipox;
		int shipoy;
		int shipx;
		int shipy;
		int shipspeed;
		char far *saveback;

	 } shipanim[40];

void initarray(void)
{


	int c,sn,tempx,tempy;
	randomize();

	for (c=0 ; c<numspr; c++)
	{
		tempx=random(319);
		tempy=random(199);

		shipanim[c].shipx=tempx;
		shipanim[c].shipy=tempy;

		shipanim[c].shipspeed=random(3)+1;

		PutImage(shipanim[c].shipx, shipanim[c].shipy, c);
		}

}


void main(void)
{
	int i,direction=1,xx,swav;

	Init_Mode();
	initarray();

	do
	{
		  for (xx=0; xx<numspr; xx++)
			{

				spx = shipanim[xx].shipx;
				spy = shipanim[xx].shipy;;
				shipanim[xx].shipox = spx;
				shipanim[xx].shipoy = spy;

				spx -= shipanim[xx].shipspeed;

				if (spx >= 319)
				{

			spx=1;
			}

				shipanim[xx].shipx = spx;
				PutImage(shipanim[xx].shipox,spy,spritenum);
				PutImage(spx,spy,spritenum);
		}
		delay(5);

	} while (!kbhit());
	/* free memory */
	for (i=0 ; i<numspr ; i++)
	{
		farfree(shipanim[i].saveback);
	}

	Close_Mode();
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

void CutImage(int spx,int spy, int spritenum)
{
	int i=0;
	offset=spy*320+spx;

	for (x=0;x<6;x++)
	{
		for (y=0;y<6;y++)
		{
			*(shipanim[spritenum].saveback+i)=*(screen+y+offset);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}


}

void PutImage(int spx,int spy,int spritenum)
{
	offset=spy*320+spx;

	*(screen+y+offset)^=11;


}

void RestoreBack(int spx,int spy, int spritenum)
{
	int i=0;
	offset=spy*320+spx;

	for (x=0;x<6;x++)
	{
		for (y=0;y<6;y++)
		{
			 *(screen+y+offset)=*(shipanim[spritenum].saveback+i);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}

}

void copypage(char *source, char *destin)
{
//	printf("Start ASM");
	asm {
		 push ds
		 cld
		 cli
		 lds si, [source]
		 les di, [destin]
		 mov cx, 16000
		 rep movsw
		 sti
		 pop ds
		 }
}




int getkey()
{

	getchar();

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

