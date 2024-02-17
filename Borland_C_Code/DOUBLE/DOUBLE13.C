/*
   double13.c

   Double buffering in VGA mode 13h (320x200, 256 colors)

   Davor Slamnig, 11/94
   (slama@slama.pub.hr)

   BC++ 3.1 - small model

   VGA is set to "big map" mode.
   The video memory starts at a000h and is 128K long.
   Page 2 begins at b000h. Page 1 one is filled with blue,
   page 2 with red (system colors).
   The program first switches pages using BIOS, then
   does a smooth scroll through the pages using VGA
   registers to change screen start address.

   Disclaimer: May not work with your card.
*/

#include <dos.h>
#include <mem.h>

#define VIDEO           0x10                              /* video int  */
#define VGAPTR          ((unsigned char far*)0xa0000000)  /* video base */
#define VGAPTR_2        ((unsigned char far*)0xb0000000)  /* page 2     */

typedef struct palette{
   unsigned char R;
   unsigned char G;
   unsigned char B;
}PALETTE;

unsigned char far* VGA_screen = VGAPTR;
unsigned char Video_page = 0;

PALETTE Syspal[256];
unsigned char Old_video_mode;

void set_vmode(unsigned char mode)
{
   union REGS regs;

   regs.h.ah = 0;             /* set video mode */
   regs.h.al = mode;

   int86(VIDEO, &regs, &regs);
}

unsigned char get_vmode(void)
{
   union REGS regs;

   regs.h.ah = 0x0f;             /* get video mode */

   int86(VIDEO, &regs, &regs);

   return regs.h.al;
}

void big_map(void)
{                          /* from Kliewer's book */
    outportb(0x3ce, 6);    /* Graphics 1 and 2 Address register,
                           index 6 - Miscellaneous  */
    outportb(0x3cf, 1);    /* graphics mode on, chain off,
                              memory map: a000h/128K */
}

void set_vpage(unsigned char video_page)
{
   union REGS regs;

   regs.h.ah = 5;                /* set active display page */
   regs.h.al = (video_page);
   int86(VIDEO, &regs, &regs);   /* call BIOS */
}

void get_palette(PALETTE *color)
{
   int k;

   disable();

   for(k = 0; k < 256; ++k, ++color){
      outportb(0x3c7, k);
      color->R = inportb(0x3c9) & 0x3f;
      color->G = inportb(0x3c9) & 0x3f;
      color->B = inportb(0x3c9) & 0x3f;
   }
   enable();
}

void set_palette(PALETTE *color)
{
   int k;

   disable();

   for(k = 0; k < 256; ++k, ++color){
      outportb(0x3c8, k);
      outportb(0x3c9, color->R);
      outportb(0x3c9, color->G);
      outportb(0x3c9, color->B);
   }
   enable();
}

void draw_map(void)
{
   _fmemset(VGAPTR, 1, 64000U);
   _fmemset(VGAPTR_2, 4, 64000U);
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

main(void)
{
   int k;

   Old_video_mode = get_vmode();
   get_palette(Syspal);
   set_vmode(0x13);
   big_map();
   VGA_screen = VGAPTR;
   set_vpage(0);

   draw_map();

   for(k = 0; k < 7; ++k){
      if(Video_page == 0){
         VGA_screen = VGAPTR_2;
         Video_page = 2;
      }
      else{
         VGA_screen = VGAPTR;
         Video_page = 0;
      }
      set_vpage(Video_page);
      delay(300);
   }

   scroll();

   set_vmode(Old_video_mode);
   set_palette(Syspal);

   return 0;
}

