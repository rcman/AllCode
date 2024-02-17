
#include <stdio.h>
#include <alloc.h>
#include <dos.h>
#include <stdlib.h>

void Init_Mode(void);
void Close_Mode(void);
void copycentre(char far *source, char far *dest, int ll, int nl, int av);
void GetMem(void);


char far *screen = MK_FP(0xa000,0);
char far *dest;



void main(void)
{
	Init_Mode();
	GetMem();
	copycentre(dest, screen, 2, 2, 10);
	Close_Mode();


}


void copycentre(char far *source, char far *dest, int ll, int nl, int av)
{
	asm {
		 .386
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

void GetMem(void)
{

	dest = (char far *) farmalloc(2000);

	if (dest == NULL)
	{
		 printf("Not enough memory to allocate screen buffer. Press any key to Exit \n");
		 exit(1);  /* terminate program if out of memory */
	}

}

