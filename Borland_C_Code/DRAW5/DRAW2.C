#include <conio.h>
#include <stdio.h>
#include <alloc.h>
#include <dos.h>
#include <process.h>
#include <stdlib.h>
#include <mem.h>
#include "draw.h"

#pragma inline

extern char far *screen;
extern offset;
extern j;
char far *st1;
extern int x,y,xy1,xy,fl,tm,width,ym,xm,button;
extern char far *savearea;
int nx,ny,val,mspeed,oldoffset,oldy,oldval,tm,oval,ptflag;
int acxm,acym,oy;          // Actuall Y Mouse and X mouse

FILE *in, *out, *outdata, *indata;


static char box[6][6] = {
    { 0x0a, 0x0a, 0x0a, 0x0a,0x0a, 0x0a, },
    { 0x0a, 0x00, 0x00, 0x00,0x00, 0x0a, },
    { 0x0a, 0x00, 0x00, 0x00,0x00, 0x0a, },
    { 0x0a, 0x00, 0x00, 0x00,0x00, 0x0a, },
    { 0x0a, 0x00, 0x00, 0x00,0x00, 0x0a, },
    { 0x0a, 0x0a, 0x0a, 0x0a,0x0a, 0x0a, },
      };
static char boxmask[6][6] = {
    { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, },
    { 0x10, 0x00, 0x00, 0x00, 0x00, 0x10, },
    { 0x10, 0x00, 0x00, 0x00, 0x00, 0x10, },
    { 0x10, 0x00, 0x00, 0x00, 0x00, 0x10, },
    { 0x10, 0x00, 0x00, 0x00, 0x00, 0x10, },
    { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, },

      };



void ptext(void)
{


    clrscr();
    cprintf("\r\n");
    textcolor(4);
    cprintf("Draw 96\r\n");
    cprintf("Copyright 1996(C)\r\n");
    textcolor(11);
    cprintf("Program Written By:\r\n");
    cprintf("Franco Gaetan & Sean Godsell\r\n");
    cprintf("This Program is NOT FREE\r\n");
    cprintf("Enter Password:");
    getch();



}

void Init_Mode(void)
{
        printf("Initialize 320 x 200 256 Colours\n");
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
        printf("Graphics Screen Closed Thank You\n");

}


void helpscreen(void)
{


    clrscr();
    cprintf("\r\n");
    textcolor(4);
    cprintf("Help Screen\r\n");
    cprintf("\r\n");
    textcolor(11);
    cprintf("Keys \r\n");
    cprintf("   C       - Copy Object\r\n");
    cprintf("   E       - Erase Object\r\n");
    cprintf("   F       - Flip Object\r\n");
    cprintf("   T       - Turn Object\r\n");
    cprintf("   +       - Move Forward one Object\r\n");
    cprintf("   -       - Move Backward One Object \r\n");
    cprintf("   PgUp    - Move Up One Row\r\n");
    cprintf("   PgDn    - Move Down One Row\r\n");
    cprintf("   S       - Save Data\r\n");
    cprintf("   L       - Load Data\r\n");
    cprintf("   Q       - Quit Program\r\n");
    cprintf("   V       - View All Objects \r\n");
    cprintf("   M       - Move the Object \r\n");
    cprintf("   U       - Undo Last Change\r\n");
    cprintf("   A       - Animate to and from an Object \r\n");
    cprintf("   F1      - Help Screen \r\n");
    cprintf("\r\n");
    cprintf("   Press Any Key for Draw ");
    getch();



}

void Calc_Coords(void)
{
	   int sval=5;
	   xm/=2;
	   mspeed=2;
	   xm = xm / sval;
	   ym = ym / sval;
	   acxm = xm;                   // Coords for little box
	   acym = ym;
	   xm = xm * sval+1;
	   ym = ym * sval;

	   ny = ym * 320;
	   val = xm + ny;
	   oval = val;

	  // tim = oval;

	   gotoxy (1,25);         //       Calculate Start of gride 10 lines down
//         printf("%d %d",oxm,xm);



}

void Init_Screen(void)
{

	if ((savearea = (char far  *) farmalloc(350)) == NULL)
	{
	printf("Not enough memory to allocate buffer\n");
	exit(1);  /* terminate program if out of memory */
	}



}

void initarray(void)
{

	int i,j,s;
	char far *p;

	if ((p=(char far *)farmalloc(sizeof (struct fshape)*TOTALSHAPE*TOTALANIMS))==NULL)
	{
		printf("Could Not Allocate Memory for Objects\n");
		exit(1);
	}

	_fmemset(p,0,sizeof(struct fshape)*TOTALSHAPE*TOTALANIMS);

	st1 = p;

	for (i=0;i<TOTALANIMS;i++)
	{
		for (j=0;j<TOTALSHAPE;j++)
			{
			animobjects[i].fshp[j]=(struct fshape *)p;
			p+=sizeof(struct fshape);

			}
	}

	// anims[i].fshp[j]->w=15;
}


void PutColor()
{
	int ad,j,n,v;
	v=0;
       //	n=0;
	ad=0;
	for (j=0;j<256;j++)
	{
	   for (x=0;x<4;x++)
	   {
		for (y=0;y<4;y++)
		{
		  *(screen+y+ad+offset+n+321)=j;
		}
		offset = offset + 320;      //bytes to next line
	   }
	 //  getch();
	 ad=ad + 5;                     // distance between colors

	 if (ad > 314)                  // if at end go to next line
	 {

		v = v + 5;
		ad = 0;
		n = 320 * v;


	 }
	 offset =0;

	 }
}

void BoxImage()

{



	for (x=0;x<6;x++)
	{
		for (y=0;y<6;y++)
		{
		    *(screen+y+offset+val)^=box[y][x];
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
	oy=width;
}

void BoxMask()

{

	for (x=0;x<6;x++)
	{
		for (y=0;y<6;y++)

		{
		  *(screen+y+offset+oldval)^=boxmask[y][x];
		}
		offset = offset + 320;      //bytes to next line
	}

	offset=0;
}

int Get_Mouse(int command)
{
      int found=0;

      if (command)
      {

	      asm     {
		      push    ds
		      mov     ax,0
		      int     0x33
		      mov     [found],ax
		      pop     ds
		      }

		  return found;
	  }

	asm     {
		push    ds
		mov     ax,3
		mov     bx,3
		int     0x33
		mov     [ym],dx
		mov     [xm],cx
		and     bx,7                        // if 0 then no button
		mov     [button],bx                 // if 1 the left button
		pop     ds
		}

	return 0;
}

void CutImage(void)
{
	int i=0;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
			*(savearea+i)=*(screen+y+offset+val);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}

	offset=0;
}

void PutImage(void)
{

	for (x=0;x<4;x++)
	{
		for (y=0;y<4;y++)
		{
		  *(screen+y+offset+val)=j;
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;

}

void RestoreBack(void)
{
	int i=0;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
			 *(screen+y+offset+val)=*(savearea+i);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
}

int filewrite(void)
{

   int i,numobj,numrow;


   if ((out = fopen("fileout.dat", "wb"))
	   == NULL)
   {
	  fprintf(stderr, "Cannot open output file.\n");
	  return 1;
   }

	if ((outdata = fopen("filedata.dat", "wb"))
		   == NULL)
		{
		  fprintf(stderr, "Cannot open output file.\n");
		  return 1;
		}
		fwrite(animobjects,sizeof(animobjects),1,outdata);


	for (numrow=0;numrow<10;numrow++)
	{
		for (numobj=0;numobj<=animobjects[numrow].max;numobj++)
		{
			if (animobjects[numrow].fshp[numobj]->flag==1)
			{

			  fwrite(animobjects[numrow].fshp[numobj],272l,1,out);

			}
		}
	}

   fclose(out);
   fclose(outdata);
   return 1;

}

int fileread(void)
{

	int i,j,numobj,numrow;
	char far *p;

   if ((in = fopen("fileout.dat", "rb"))
	   == NULL)
   {
	  fprintf(stderr, "Cannot open input file.\n");
	  return 1;
   }

	if ((indata = fopen("filedata.dat", "rb"))
		   == NULL)
		{
		  fprintf(stderr, "Cannot open output file.\n");
		  return 1;
		}

		fread(animobjects,sizeof(animobjects),1,indata);

	p = st1;

	for (i=0;i<TOTALANIMS;i++)
	{
		for (j=0;j<TOTALSHAPE;j++)

			{
			animobjects[i].fshp[j]=(struct fshape *)p;
			p+=sizeof(struct fshape);

			}
	}


	for (numrow=0;numrow<10;numrow++)
	{
		for (numobj=0;numobj<=animobjects[numrow].max;numobj++)
		{

		fread(animobjects[numrow].fshp[numobj],272l,1,in);
		}
	}





   fclose(in);
   fclose(indata);


   return 1;



}

