#include "alloc.h"
#include "io.h"
#include "fcntl.h"
#include "conio.h"
#include "sys/stat.h"

#include "fade.h"

char pal[768];
char pal2[768];



const char *image="satelite.raw";
const char *palf1="pal1.pal";
const char *palf2="pal2.pal";

int readstuff(const char *filename,void far *buf,unsigned length)
{
   int handle, bytes;


   if ((handle =
      sopen(filename, O_RDONLY | O_BINARY, S_IWRITE | S_IREAD)) == -1)
   {
      printf("Error Opening File\n");
      exit(1);
   }

   if ((bytes = read(handle, buf, length)) == -1) {
      printf("Read Failed.\n");
      exit(1);
   }
   return 0;
}



void main()
{  int n;
   asm{           /* set mode 13h video mode*/
     mov  ax,13h
     int  10h
   }

  /* read 320*200 raw image to video buffer*/

   fill_pal(&pal,0,0,0);  /* fill pal[768] with 0*/
   setPal(&pal);
   readstuff(image,(void far *)0xa0000000,64000);

  /*read palette file */
   readstuff(palf1,&pal,768);
   readstuff(palf2,&pal2,768);

  /* note: fade_between_screen, fade_screen, bright_screen all
     change the content of the palette array */
   gotoxy(1,22);
   printf("fade_in         ");
   fade_in_screen(&pal);
   getch();

   gotoxy(1,22);
   printf("palette morphing");
   fade_between_screen(&pal,&pal2);
   getch();

   gotoxy(1,22);
   printf("brightedn out   ");
   bright_out_screen(&pal);

   getch();

   readstuff(palf1,&pal,768); /* let's load the palette data again*/
   gotoxy(1,22);
   printf("bright in       ");
   bright_in_screen(&pal);
   getch();

   gotoxy(1,22);
   printf("palette rotation");
   n=0;
   do
   {rotate_pal(&pal,128+16,16,n);
    if(n==15)
     n=0;
    else
     ++n;
   }while(!kbhit());

   getch();

   gotoxy(1,22);
   printf("fade out        ");
   fade_out_screen(&pal);


   asm{         /* return to text mode */
     mov ax,3h
     int 10h
   }
   printf("fadedemo.exe written by Esak 1994");
}