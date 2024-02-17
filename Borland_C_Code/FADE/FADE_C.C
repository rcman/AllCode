#include <alloc.h>

#include "fade.h"


void fade_out_screen(void far *pal)
{ int n;
  for(n=0;n<32;++n)
   fade_out_once(pal);
}

void bright_out_screen(void far *pal)
{ int n;
  for(n=0;n<32;++n)
   bright_out_once(pal);
}


void fade_in_screen(void far *pal)
{ int n;
  char far *buff;
  buff=farcalloc(768*2,1);
  for(n=0;n<32;++n)
   fade_in_once(buff,pal);
  farfree(buff);
}

void bright_in_screen(void far *pal)
{  int n;
   char far *buff,*buff2;
   buff=farmalloc(768*2);
   buff2=farmalloc(768);
   copyPal(pal,buff2);

   fill_pal(buff,63,63,63);  /* fills the palette array with white color */
   sub_palette(buff,buff2);
   for(n=0;n<63;++n)
    fade_between_once(buff,buff2);
   farfree(buff2);
   farfree(buff);
}


void fade_between_screen(void far *pal,void far *paldest)
{  int n;
   char far *buff,*buff2;
   buff=farmalloc(768*2);
   buff2=farmalloc(768);
   copyPal(paldest,buff2);
   copyPal(pal,buff);
   sub_palette(buff,buff2);
   for(n=0;n<63;++n)
    fade_between_once(buff,buff2);
   farfree(buff2);
   farfree(buff);
}

