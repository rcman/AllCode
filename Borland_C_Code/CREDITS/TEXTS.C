// By: Franco Gaetan
// Date Start: Dec 2, 1996
//
//


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

#define TOTALSHAPE 100
#define TOTALANIMS 10
#define TOTALOBJECTS 20

// #pragma inline

void EraseDestin(void);
void initscreen(void);
void copypage(char *source, char *destin);
void PlaceLine(int line);
void checkletter(int line);
void Init_Mode(void);
void initarray(void);
void Close_Mode(void);
int getch(void);
void initshapes(void);
void nextshape(void);
void moveship(void);
void PutImage(int spx,int spy);
void CheckImage(int line);
int fileread(void);
int getk(void);
void checkshots(void);
void moveshots(void);


//Global

char far *screen = 0xa000,0);
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

//////////////////////////

int top=19;
int bottom=190;	// bottom let add X to it to get shape
int spaceleftb=0;
int spaceleftt=0;
int leng=0;		// Length of String Start
int line=0;
char *pc[20];
int checkline=0;
int nextline=0;
unsigned int new=0;


FILE *in, *out, *outdata, *indata, *std, *map, *mapin;




struct anim
	{

		int x,y;
		int n,c;
		int flag;
		int rowflag;
		char far shp[260];
					// TOTALSHAPE  =  20
	} anims[TOTALSHAPE];

struct fshape
	{

		int w,h;
		int n,c;
		int flag;
		int rowflag;
		char far shp[260];
					// TOTALSHAPE  =  20
	} fshp[TOTALSHAPE];

struct animshape {
		int active;
		int animwidth;
		int animheight;
		int animox;
		int animoy;
		int animx;
		int animy;
		int prox;
		int animspeed;
		int currentshape;
		int oldshape;
		int max;
		int row;

		struct fshape *fshp[TOTALSHAPE];        // 10 Pointers to data

	 } animobjects[TOTALANIMS];



void main()
{
	int i;

	pc[0] = "Thisis 1";
	pc[1] = "welcoe";
	pc[2] = "Conatco";
	pc[3] = "httpeco";
	pc[4] = "Thaniom";
	pc[5] = "you bred";
	pc[6] = "Hi tiends";

	Init_Mode();
	initscreen();
	fileread();					// Read My Letters In

	do {

		checkletter(line);		// Check length of Line and Start
		PlaceLine(line);

				 do {

				  CheckImage(line);
				  copypage(destin, screen);

				  EraseDestin();

				  checkline = checkline + 1;
						if (checkline ==10)
						{
						 nextline=1;
						 checkline=0;
						}


				 }	while(nextline!=10);

		 line = line + 1;
		 //getch();
		 }while(line!=8);


		getch();
		farfree(destin);
		Close_Mode();

}

int fileread(void)
{

	int s=0,i;
   struct fshape *p;

	if ((in = fopen("mylet4.std", "rb"))
		 == NULL)
	{
		fprintf(stderr, "Cannot open input file.\n");
		return 1;

	}

	i=0;
	for (s=0,p=&fshp[s];s<TOTALSHAPE && feof ;s++,p=&fshp[s],i=0)

	while (i<256)
		{
	*(p->shp+i)=fgetc(in);
  //      printf("%x",*(p->shp+i));
	i++;
		}

	fclose(out);

	return 1;

}

void initscreen(void)
{

		destin = (char far  *) farmalloc(64000l);
		 memset(destin, 0, 64000l);

	if (destin == NULL)
		{
			  printf("Not enough memory to allocate screen buffer\n");
			  exit(1);  /* terminate program if out of memory */
		}

}



void PutImage(int spx,int spy)
{
	offset=spy*320+spx;

	for (x=0;x<8;x++)
	{
		for (y=0;y<8;y++)
		{
		  *(screen+y+offset)^=anims[2].shp[x*16+y];
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


void CheckImage(line)
{

	int i;
  //	printf("Loop %d ",animobjects[line].max);

	for (i=0;i<=animobjects[line].max;i++)
	{
 //	  printf("X= %d   Y=  %d\n",animobjects[i].animx,animobjects[i].animy);
		spx = animobjects[i].animx;
		spy = animobjects[i].animy;

		offset=(spy*320)+spx;
		spy = spy -1;
		animobjects[i].animy=spy;

		spaceleftb = bottom - spy;
		spaceleftt = top - spy;

			 if (spaceleftb <=0)
			 {
			 spaceleftb=0;
			 }
				 //getch();


						if (spaceleftt <=0)
						{
						spaceleftt=0;
						}
							  if (spaceleftt >=15)
							  {
								spaceleftt=16;
							  }

			for (x=spaceleftt;x<spaceleftb;x++)
			{
				  for (y=0;y<16;y++)
				  {
					*(screen+y+offset)^=fshp[1].shp[x*16+y];
//					anims[1].shp[x*16+y];
					}
					 offset = offset + 320;      //bytes to next line
			 }
	 }
	getch();
}

void checkletter(int line)
{

	int i,j=0;

	char p;
	leng=0;


	// p = *(pc[0]+j);

	do {

		p = *(pc[line]+leng);
		leng +=1;


	}while(p!=0);

	animobjects[line].max=leng;
//	printf("%d max ",animobjects[line].max);

	leng = leng / 2;
	leng = leng * 8;
	leng = 160 - leng;
	animobjects[line].prox=leng;
	printf("length = %d",animobjects[line].prox);


	//printf("%d",leng);
	//getch();


	//printf("%d",p);
	// Letter a = 97 so subtract 97 from array value
	// NULL = 0
	//
}

//10 charter 8 bit + 2
// centre 160
// devide length of string / 2
// multiple by 8
// subtract 160

void PlaceLine(line)
{

	char p;
	int i,v,q;
	int over=0;

	//newval = animobjects[line].prox;

	spy = 180;		// Offscreen so you can't see
 //	new = spy *320+newval;

//	offset=(spy*320)+animobjects[line].prox;
//	leng = animobjects[line].max;
	 //	printf("%d max2 ",animobjects[line].max);

	for  (i=0;i<=animobjects[line].max;i++)
	{
	//getch();
			//  printf("X= %d   Y=  %d\n",animobjects[i].animx,animobjects[i].animy);
	//  getch();

	  p = *(pc[line]+i);
	  q = (int)p;
	  v = q - 97;				// Letter 'a' is 97

 //		printf("%d",q);
 //		getch();

		v =1;
		animobjects[i].animx=animobjects[line].prox+over;
		animobjects[i].animy=spy;

			for (x=0;x<16;x++)
			{
				for (y=0;y<16;y++)
				{
				*(screen+y+offset+over)=fshp[v].shp[x*16+y];
			 //	anims[v].shp[x*8+y];
				 }
				 offset = offset + 320;      //bytes to next line
			 }

			 over = over + 9;
			 offset = spy*320+animobjects[line].prox;
			 spy=180;
 }


}

void copypage(char *source, char *destin)
{
//      printf("Start ASM");
	asm {
		 //.386
		 push ds
		 cld
		 lds si, [source]
		 les di, [destin]
		 mov cx, 32000
		 rep movsw
		 pop ds
		 }
}

void EraseDestin(void)
{
	int y;
	for (y=0;y<=64000;y++)
	{
	*(destin+y)=0;
	}


}



/*
	for (i=0;i<TOTALANIMS;i++)
	{
		for (j=0;j<TOTALSHAPE;j++)
			{
			animobjects[i].fshp[j]=(struct fshape *)p;
			p+=sizeof(struct fshape);

			}
	}

  */