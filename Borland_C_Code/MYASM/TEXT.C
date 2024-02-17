#include <conio.h>
#include <stdio.h>
#include <stdio.h>
#include <dos.h>
#include <bios.h>


#pragma inline

typedef unsigned char byte;
typedef unsigned int  word;

void copypage(char *source, char *dest);

extern char far *dest;

void ptext(void)
{


	 clrscr();
	 cprintf("\r\n");
	 textcolor(4);
	 cprintf("Draw 96\r\n");
	 cprintf("Copyright 1996(C)\r\n");
	 textcolor(11);
	 cprintf("Program Written By:\r\n");
	 cprintf("Franco Gaetan\r\n");
	 cprintf("This Program is NOT FREE\r\n");
	 cprintf("Enter Password:");


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
   // cprintf("\r\n");
    textcolor(4);
    cprintf("Help Screen\r\n");
   // cprintf("\r\n");
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
	 cprintf("   F8      - Layout Screen \r\n");
    cprintf("   F2      - Copy Object to another Row and Object \r\n");
    cprintf("   F5      - Save Row (Cut / Copy / Paste ) For LOAD/SAVE \r\n");
    cprintf("ArrowKeys  - Move Object UP/DOWN/LEFT/RIGHT \r\n");

    cprintf("\r\n");
	 cprintf("   Press Any Key for Draw ");
	 getch();

		

}

void introscreen(void)
{


    clrscr();
   // cprintf("\r\n");
    textcolor(4);
    cprintf("GC");
    textcolor(1);
    cprintf("Software\r\n");
   // cprintf("\r\n");
    textcolor(11);
    cprintf("Products 1996 \r\n");
    textcolor(12);
    cprintf("This Product is Shareware CopyRight(c) 1996 \r\n");
    cprintf(" \r\n");
    textcolor(11);
    cprintf("This product is NOT FREE! The program took many hours\r\n");
    cprintf("to write. Respect the fact that this is the only way I make\r\n");
    cprintf("a living. I hope you enjoy the program and find it useful.\r\n");
    cprintf("If you find a bug or problem please feel free to write me\r\n");
    cprintf("and I'll see what I can do for you. \r\n");
    cprintf(" \r\n");
    textcolor(4);
    cprintf("GCDraw Version 1\r\n");
    textcolor(11);
    cprintf("     \r\n");
    cprintf("If you have use for the program and I hope you will send\r\n");
    cprintf("5$ CDN to the Address below. GCDraw was written because I\r\n");
	 cprintf("couldn't find and sprite maker programs to fit my needs. So\r\n");
    cprintf("I decided to write my own and in comparison I think I did \r\n");
	 cprintf("just fine. Hope you like it. \r\n");
    cprintf("\r\n");
    cprintf("			Address:\r\n");
    cprintf("				Bla Bla Bla\r\n");
    cprintf("  \r\n");
    textcolor(4);
    cprintf(" http://www.inforamp.net/~franco \r\n");
    cprintf(" email: franco@inforamp.net \r\n");
    cprintf(" \r\n");

	 cprintf("\r\n");
    cprintf("   Press Any Key for Draw ");
    getch();



}


void WaitRetrace() {

  _DX = 0x03DA;

  l1: asm {
	in  al,dx;
	and al,0x08;
	jnz l1;
      }

  l2: asm {
	in  al,dx;
	and al,0x08;
	jz  l2;
      }
}


void copypage(char *source, char *dest)
{
	asm {
		 ba dw ?
		 sb dw ?
		 push ds
		 cld
		 lds si, [source]
		 les di, [dest]
		 mov cx, 16000
		 db 0x66
		 rep movsw
		 pop ds


		 }
		 // Source is gonna be:
		 // On the Move for Double word (4 Bytes) the screen will be as follows
		 // 32 pixels(bytes) in from the end of the screen and 32 down at start.
		 // The End will be 9 blocks of 16 *320 + 16 * 16
		 // Each block is 16 x 16
		 // 16 * 320 x how many blocks down 9
		 // Plus how many accross 16 pixels wide time 16 blocks
		 // Its 32 bytes to the next line of data
		 // Its 8 4 byte counts in from start and 8 4 bytes counts from end.
		 // Double word is half that - 32

}

void copycentre(char far *source, char far *dest, int ll, int nl, int av)
{
	asm {
		 push ds
		 push	cx
		 push	bx
		 push	dx
		 push	es
		 cld
		 mov 	cx, [ll]
		 mov 	dx, [nl]
		 mov 	bx, [av]
		 les 	di, [dest]
		 lds 	si, [source]

  cont:push	cx
		 db	0x66
		 rep 	movsw
		 pop 	cx
		 add	si,bx
		 add	di,bx
		 dec	dx
		 cmp	dx,0
		 jne 	cont
		 pop	es
		 pop	dx
		 pop	bx
		 pop	cx
		 pop 	ds

		 }
}




/*
cprintf("GCDraw was a program I made from nessity rather that will\r\n");
	 cprintf("It was originally written in QBasic and it got real slow.\r\n");
	 cprintf("So I started writing it in C and added many new fetures, many\r\n");
	 cprintf("I hope you'll like. I'm realeasing this 16 pixel version as \r\n");
    cprintf("I'm now working on a 32 pixel version for later release. \r\n");
    cprintf("I hope you find the DOCS useful, Ive included anything I can\r\n");
    cprintf("think of including sample graphics and source code in C for \r\n");
    cprintf("putting it in your programs. I can be reached at the following:\r\n");
    cprintf("  \r\n");
    textcolor(4);
    cprintf(" http://www.inforamp.net/~franco \r\n");
    cprintf(" email: franco@inforamp.net \r\n");
    cprintf(" \r\n");

    cprintf("\r\n");
    cprintf("   Press Any Key for Draw ");
    getch();
*/
