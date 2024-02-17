// Move an object through an array
// Date Start: Feb 16, 1996
// Rev: June 7 - 96
//
//
//
//
//**********************************************************

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <bios.h>
#include <conio.h>

#define TOTALSHAPE 10
#define TOTALANIMS 10
#define TOTALOBJECTS 20


// #include <complex.h>


//#include <graphics.h>


void Init_Mode(void);
void initarray(void);
void Close_Mode(void);
int getch(void);
void initshapes(void);
void nextshape(void);
void moveship(void);
void PutImage(int spx,int spy);
int fileread(void);
int getk(void);
void checkshots(void);
void moveshots(void);


//Global

char far *screen = MK_FP(0xa000,0);
char far *destin;

//unsigned char key;
int key;
int ckey;

int spx=0;            // Pixels
int spy=0;
int x,y,xy1,xy,fl,tm,s,t,l;
int offset = 0;
int numspr=10;
int shotmx[100];
int shotmy[100];

int ov=180*320;
int shipx = 0;
int shotfired=0;
int shoty=180;
int shottotal=0;
int tempx,tempy;
int svalue=0;


FILE *in, *out, *outdata, *indata, *std, *map, *mapin;

struct savebg

	 {
	 int bgflag;
	 char far backgnd[256];

	 } savedarea[TOTALOBJECTS];

struct anim
	{

		int w,h;
		int n,c;
		int flag;
		int rowflag;
		char far shp[260];
					// TOTALSHAPE  =  20
	} anims[TOTALSHAPE];

struct ship {
		int shipox;
		int shipoy;
		int shipx;
		int shipy;
		int shipspeed;
		char far *saveback;

	 } shipanim[40];


struct fshape
	 {

		 int w,h;
		 int n,c;
		 int flag;
		 int rowflag;
		 char far shp[260];

					 // TOTALSHAPE  =  20
	 };



void main()
{
	Init_Mode();
	fileread();
	initshapes();

	do {

		getk();
		moveship();
  //	delay(5);
		checkshots();
		moveshots();

		delay(1);

	}while(key!=27);

	Close_Mode();

}


int getk(void)
{
/*	asm {
		mov ah,0
		in al,0x60
		mov [key],al
		 }
*/			 // 75 left
			 // 77 Right
		//      printf("%d",key);

			// delay(20);
		if (_bios_keybrd(_KEYBRD_READY))
		{
			key = _bios_keybrd(_KEYBRD_READ);
			if ((key & 0xff) == 0)
				key >>= 8;
			else
				key &= 0xff;
		}
		else
			ckey = _bios_keybrd(_KEYBRD_SHIFTSTATUS) & 0xff;
//			printf ("%X\n", key);
		if (key == 75)
			shipx = shipx - 1;
		if (key == 77)
			shipx = shipx + 1;

		if (ckey==4)
		{
			shotfired=1;
		}

		return(key);
}

int fileread(void)
{

	int s=0,i;
   struct anim *p;

   if ((in = fopen("gal2.std", "rb"))
       == NULL)
   {
      fprintf(stderr, "Cannot open input Standard file.\n");
      return 1;
   }

   i=0;
   for (s=0,p=&anims[s];s<TOTALSHAPE && feof ;s++,p=&anims[s],i=0)

	while (i<256)
		{
	*(p->shp+i)=fgetc(in);
	i++;
      }

   fclose(out);

   return 1;

}


void nextshape(void)
{

	int i,j,vz,a=0;
	int ov=0;

	vz=0;

	for (a=0;a<30;a++)
	{

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{
		*(screen+j+vz+ov) = anims[1].shp[i*16+j];

		}
		vz = vz + 320;
			 //       getch();
	    }
	    getch();
	    vz=0;
	    ov=ov+21;
	}

}

void initshapes(void)
{

	int i,j,vz;


	vz=0;

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{
		*(screen+j+vz+ov) = anims[2].shp[i*16+j];

		}
		vz = vz + 320;
		 }

}


void moveship(void)
{

	int i,j,vz;

	vz=0;
	if (shipx >=300)
		shipx=300;
	if (shipx <=17)
		shipx=17;


	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{
		*(screen+j+vz+ov+shipx)=anims[1].shp[i*16+j];

		}
		vz = vz + 320;
		 }

}










void CutImage(int spx,int spy, int spritenum)
{
	int i=0;
	offset=spy*320+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
			*(shipanim[spritenum].saveback+i)=*(screen+y+offset);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}


}







void PutImage(int spx,int spy)
{
	offset=spy*320+spx;

	for (x=0;x<3;x++)
	{
		for (y=0;y<16;y++)
		{
		  *(screen+y+offset)^=anims[2].shp[x*16+y];
		}
		offset = offset + 320;      //bytes to next line
	}

}

void RestoreBack(int spx,int spy, int spritenum)
{
	int i=0;
	offset=spy*320+spx;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
			 *(screen+y+offset)=*(shipanim[spritenum].saveback+i);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}

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


void checkshots(void)
{
	// int shots[50][50];

	if (shotfired ==1)
	{

		shotmx[shottotal]=shipx;
		shotmy[shottotal]=shoty;

		shottotal = shottotal + 1;
		shotfired = 0;



	}






}

void moveshots(void)
{

	int i;

	for (i=0;i<shottotal;i++)
	{
		tempx =	shotmx[i];
		tempy = shotmy[i];

	//	SyncToVerticalRetrace();
		PutImage(tempx,tempy-1);

		PutImage(tempx,tempy);
		tempy = tempy -1;
		if (tempy < 10)
		{

			shottotal = shottotal - 1;

		}

		shotmy[i]=tempy;
	  //	delay(10);

	 }

}