
//               //
// INCLUDE FILES //
//               //

  #include <conio.h>
										// clrscr(), getch(), kbhit()
  #include <alloc.h>
  #include <iostream.h>
									// cout
  #include <stdlib.h>
									// rand()
  #include "gfx2.cpp"

void far *farcalloc(unsigned long nunits, unsigned long unitsz);

  // unsigned chars are sissy
  typedef unsigned char byte;
  typedef unsigned int  word;
  char far *buff;

  extern byte R;   // 0-63
  extern byte G;   // 0-63
  extern byte B;   // 0-63

void main()
{

		int i;
		byte v;
		buff=(char far *)farcalloc(768,1);
		SetMCGA();

		for (i=0;i<768;i+=3)
		{
			Pal PalGet(v);
		  //	*mypal(i)={byte)R;
		  //	*mypal(i+1)=(byte)G;
		  //	*mypal(i+2)=(byte)B;
		}


		getch();

		SetText();

}

