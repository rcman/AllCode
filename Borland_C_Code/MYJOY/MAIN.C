#include <stdio.h>
#include <dos.h>
#include <mem.h>
#include <conio.h>


#pragma inline

#define VIDEO 0x10

void set_vpage(void);
void initpages(void);
void joy(void);
void BoxImage(void);
void BoxImageII(void);
void scroll(void);
//void set_vpage(char video_page);

#define VGAPTR ((unsigned char far*)0xa0000000)
#define VGAPTR_2 ((unsigned char far*)0xb0000000)

unsigned char far* VGA_screen = VGAPTR;

unsigned char video_page = 0;

static char box[6][6] = {
    { 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, },
    { 0x0b, 0x00, 0x00, 0x00, 0x00, 0x0b, },
    { 0x0b, 0x00, 0x00, 0x00, 0x00, 0x0b, },
    { 0x0b, 0x00, 0x00, 0x00, 0x00, 0x0b, },
    { 0x0b, 0x00, 0x00, 0x00, 0x00, 0x0b, },
    { 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, },

      };


int offset=0;

int page1,page2;

int posx=0,posy=0;

void main(void)
{
	int k;

	initpages();

       //	printf("Starting\n");

	for (k=0;k<7;k++)
		{
		joy();

		printf("%d  %d ",posx,posy);


	      //	if (video_page ==0)
	      //	{
	      //	VGA_screen=VGAPTR_2;
	      //	video_page=2;

	      //	page1 = page1 +5;
	     //	BoxImage();

	     //	printf("page1\n ");
		getch();
	     //	set_vpage();
	      //	}

	      //	else
	      //	{
	     //	VGA_screen=VGAPTR;
	      //	video_page=0;
	      //	set_vpage();
	      //	page2 = page2 +16;
	      //	BoxImageII();

	      //	printf("page2\n");
	      //	getch();

		}

	      //	scroll();




}


void set_vpage()
{
   union REGS regs;

   regs.h.ah = 5;                /* set active display page */
   regs.h.al = (video_page);
   int86(VIDEO, &regs, &regs);   /* call BIOS */
}


void initpages(void)
{
      //	printf("Start");

	asm {
	    mov ax,0xff
	    out 0x201,ax


	    int 0x10
	    mov dx,0x3ce
	    mov al,6
	    out dx,al
	    inc dx                  // out portB
	    mov al,1
	    out dx,al
	    }

}



void BoxImage()

{

      int x,y;


	for (x=0;x<6;x++)
	{
		for (y=0;y<6;y++)
		{
		    *(VGAPTR+y+offset+page1)=box[y][x];
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
     //	oy=width;
}

void BoxImageII()

{

      int x,y;


	for (x=0;x<6;x++)
	{
		for (y=0;y<6;y++)
		{
		    *(VGAPTR_2+y+offset+page2)=box[y][x];
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
     //	oy=width;
}

void set_startline(int line)
{
   unsigned lline;

   lline = (unsigned)line * 80U;

   outportb(0x3d4, 0x0c); /* CRTC address (color), Start address high */
   outportb(0x3d5, lline >> 8);     /* hi-byte */

   outportb(0x3d4, 0x0d); /* CRTC address (color), Start address low */
   outportb(0x3d5, lline & 0xff);   /* lo-byte */
}

void scroll(void)
{
   int line;

   for(line = 0; line < 200; ++line){
      set_startline(line);
      delay(10);
   }

   for(; line >= 0; --line){
      set_startline(line);
      delay(10);
   }
}


void joy(void)
{
      //	printf("Start");

	asm {

	    mov al,1
	    out 0x201,al





	    }

}
