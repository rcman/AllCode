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
void draw_screen(void);
int go_through_the_array(void);
int fileread2(void);
void Current_Screen(void);



//Global

char far *screen = MK_FP(0xa000,0);
char far *destin;

unsigned char key;

void *buffer;
void *image;
unsigned int imsize;
int top = 290;
int bottom = 35;
int rbottom = 300;
int lbottom = 145;
int spx=0;            // Pixels
int spy=0;
int spritenum=0;
int lp;
int x,y,xy1,xy,fl,tm,s,t,l;
int z=0;
int offset = 0;
int numspr=10;
int ap=0;
int dn=0;
int cnt=0;
int blx=0;
int bly=0;
int temp1=0;
int temp2=0;
int temp3=0;
int temp4=0;
int flg=0;
int check=0;
int blockval=0;
int shot=0;
int jump=0;

int sp=0,rw=0;
int below;
int above;
int infront;
int rc,hc,lc,ext;
int maxscreenwidth=5; 		// 5 x 20 blocks = 100 Accross
int maxscreenheight=5;		// 5 x 10 blocks = 50  Down

int loy=0,lox=0;
int layarr[100][100];	// Row & Coloumn
int maxscreens=2;		// 5 screens accross x 5 screens down = 25
int screenx=19;
int screeny=10;
int screenystart=0;
int screenxstart=0;
int scrollx=0;
int savedobjects[20][10];
int animscreen=0;
int curval=0;
int t1=0;
int newobj;
int obj1s=35;	// Object 1 start
int obj1e=37;	// Object 1 end

char far *p;
char far *st1;



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



struct fshape
	 {

		 int w,h;
		 int n,c;
		 int flag;
		 int rowflag;
		 char far shp[260];

					 // TOTALSHAPE  =  20
	 };



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
	Init_Mode();
	fileread();
	initarray();
	fileread2();
	Current_Screen();

 //	do
 //	{
 //		getk();
 //		printf("%x\n",key);
 //    //	delay(5);
 //	}while(key!=1);


	getch();

	Close_Mode();

}

void initarray(void)
{

	int i,j,s;
	char far *q;

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
			animobjects[i].animx=100;
			animobjects[i].animy=100;

			}
	}
	for (i=0;i<=9;i++)
	{
		savedarea[i].bgflag=0;
	}
	// int layarr[100][50];	// Row & Coloumn
	for (i=0;i<100;i++)
	{
		for (j=0;j<50;j++)
			{
			layarr[i][j]=-1;	// Row & Coloumn
			}
	}






}



/*
int go_through_the_array(void)

{

	// whats in the array
	// the block that I'm on and the one in front of me
	// above me and below me
	// that is 16 * the block number for the width
	// and there are 20 blocks accross and 12 down
	//      spx = 4*16*320; // start at 0 and count 4 down the 5th
	//      spy = 16;       block is where its drawn
	//      spx = 20480


	int ap=0;
	int dn=0;
	int cnt=0;
	int test;

	offset = 0;

	for (t=0;t<10;t++)
	{
		for (s=0;s<20;s++)
		{

			temp2 = (s*16)+((t*16)*320);                    // the screen box
			temp3 = temp2+(8*320+8)    ;                    //CENTRE of screen box 16 x 16
			temp4 = ((((shipanim[0].ycor/16)*16)*320))+((shipanim[0].xcor)+(8*320+2));   // centre of my object
			offset = 0;
			test =  shipanim[0].xcor;
			ap = ap +16;
			temp1 = temp4 - temp3;

			if (temp1 <= 0)
				temp1 = temp1 * -1;            // see how close they are



		if (temp1 < 10 )        // WHERE SHIP IS compared to block
			{



				if (t==0)                                               // Most left hand side
					above=screens[t][s];
					else
					{
					above=screens[t-1][s];           // temp2 = (s*16)+((t-1*16)*320);   // ABOVE
					infront=screens[t][s+1];         // temp2 = (s+1*16)+((t*16)*320);   // IN FRONT
					below=screens[t+1][s];          // temp2 = (s*16)+((t+1*16)*320);   // BELOW
					}

					if (below==0 && jump==0)
					{
					spy = shipanim[0].ycor;
					spy = spy + 1;
					shipanim[0].ycor = spy;                 //SET VERTICAL HERE FOR NOW
					}
					else
					{
					    if (infront==0)                         //SET VERTICAL HERE FOR NOW
					    {

						spx = shipanim[0].xcor;
						spx = spx + 1;
						shipanim[0].xcor = spx;                 //SET VERTICAL HERE FOR NOW
					    }
					    if (above==0 && lc=='j')
					    {
						spy = shipanim[0].ycor;
						spy = spy - 1;
						shipanim[0].ycor = spy;                 //SET VERTICAL HERE FOR NOW
						if (jump==0) jump=90;
						jump--;

					    }

					}
					t=13;
					s=21;

				}


		}   // FOR T
		cnt = cnt + 15;
		dn = cnt * 320;
		offset=0;

	}



}

*/

int fileread2(void)
{

	int i,j,k,numobj,numrow;

 //	int indat=0;
	char far *p;
	char label[20];
	char infst[20];
	char smap[20];

	// ClearData();		// Clear the Array

	gotoxy (1,22);
	printf("Please enter Filename: ");
	fflush(stdin);  /* flush the input stream in case of bad input */

	scanf("%8s",label);

	strcpy(infst, label);
	strcpy(smap, label);

	strcat(label,".dat");

	strcat(infst,".inf");
	strcat(smap, ".map");

	if ((mapin = fopen(smap, "rb"))
	 == NULL)
	 {
	 fprintf(stderr, "Cannot open Standard file.\n");
	  return 1;
	 }


	if ((in = fopen(infst, "rb"))
	   == NULL)
	{
	  fprintf(stderr, "Cannot open input file.\n");
	  return 1;
	}

	if ((indata = fopen(label, "rb")) == NULL)
	{
	     fprintf(stderr, "Cannot open output file.\n");
	     return 1;
	}

	   for(i=0;i<100;i++)
	   {
	     for(j=0;j<100;j++)
	     {
	     fscanf(mapin,"%d",&layarr[j][i]);
	     }

	   }

	fread(animobjects,sizeof(animobjects),1,indata);


	p = st1;

	for (i=0;i<=9;i++)
	{
		for (j=0;j<=9;j++)
		{
		 animobjects[i].fshp[j]=(struct fshape *)p;
		 p+=sizeof(struct fshape);
		}

	}


	for (numrow=0;numrow<=9;numrow++)
	{

		for (numobj=0;numobj<TOTALSHAPE;numobj++)
		{
		 fread(animobjects[numrow].fshp[numobj],272l,1,in);
		}

	}

   fclose(mapin);
   fclose(in);
   fclose(indata);


   return 1;



}

void Current_Screen(void)
{
	int ap=0;
	int dn=0;
	int cnt=0;
	int t,s,y;
	long int clr;
	//int curval=0;
	float t1=0;

	rw = 0;
	sp =0;


	for (clr=0;clr<64000;clr++)
		{
		*(screen+clr)=0;
		}

	for (t=0;t<=10;t++)
	{
		for (s=0;s<=19;s++)
		{

		    curval = layarr[s][t];
		   // printf("%d",curval);
		   // getch();
		    if (curval >=0)
		    {
		    t1 = ((float)curval /10);
		    rw = t1;
		    t1 = t1 - rw;
		    t1 = t1 * 10;
		    sp = (int) (t1+.1);

		   // sp = ((t1 - rw)*10);
		    }


		    for (x=0;x<16;x++)
			{
				for (y=0;y<16;y++)
				{
				if (curval>=0)
				{
				*(screen+y+offset+ap+dn+scrollx)=animobjects[rw].fshp[sp]->shp[x*16+y];
				}
				else
				{
				*(screen+y+offset+ap+dn+scrollx)=0;
				}

				}
				offset = offset + 320;      //bytes to next line
				}

				offset = 0;
				ap = ap +16;
			}
		cnt = cnt + 16;
		dn = cnt * 320;
		offset=0;
		ap=0;


		}


	}


int getk(void)
{
	asm {
		mov ah,0
		in al,0x60
		mov [key],al
	    }




}

int fileread(void)
{

   int s=0,i;
   struct anim *p;

   if ((in = fopen("man.std", "rb"))
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

