// Drawing Program
// Date Start: June 1, 1995
// Revision: April 28 - 1996
// DRAW V 6

#include "draw.h"
#include <dos.h>
#include <bios.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <conio.h>

#pragma inline
#define TOTALSHAPE 12
#define TOTALANIMS 12
#define TOTALOBJECTS 20

void Init_Screen(void);
void initarray(void);
void CutImage(void);
void PutImage(void);
void RestoreBack(void);
void PutColor(void);
void BoxImage(void);
// void BoxMask(void);
int getch(void);
int Get_Mouse(int command);
void DrawCursor(void);
void DrawBox(void);
void DrawMask(void);
void Calc_Coords(void);
void Check_Color(void);
void DrawGrid(void);
void Colors(void);
void Check_Draw(void);
void drawmem(void);
void check_thedata(void);
void nextshape(int , int);
void drawshape(int , int);
void Draw_Data_to_Grid(int , int);
void clearshape(void);
void cleargrid(void);
void placeshape(int , int);
void draw_screen(int , int);
void layout_screen(int , int);
int filewrite(void);
int fileread(void);
void EraseObject(void);
void SaveLastImage(void);
void UndoLastChange(void);
void copyshape(void);
void flipshape(void);
void Animate(void);
void Debug(void);
void BigBox(int , int);
void Check_Select(void);
void draw_plus_minus(void);
void DrawPnt(void);
void SaveRow(void);
void DrawRow(int rw);
void dojoy(void);
void SyncToVerticalRetrace( void );

char far *screen = MK_FP(0xa000,0);

int lc,rc,hc;
int nff=0;
int flag=1;
int spy=100;
int spritenum=0;
int lp;
int x,y,xy1,xy,fl,tm,width,oy,ym,xm,button,chkdrw=0;
int j=2;                // start off color
int z=0;
int offset = 0;
int numspr=10;
char far *savearea;

int nx,ny,val,mspeed,oldoffset,oldy,oldval,tm,oval,ptflag;
int c,b,st,ac,step,tim;
int inc=5;
int lso=0;
int acxm,acym;          // Actuall Y Mouse and X mouse
char far *point;
char *p;
char far *st1;
char undoarea[260];
int sp=0,rw=0;
int allflg=0;
int layout=0;
char sc[20];
int ga=47,gb=61,gc=144,gd=126;
int sval=5;                             // step value for draw
int chgpnt=0;
int memflag=0;


int k,jx,jy;
int c,m1,m2,m3,m4,m5;

FILE *in, *out, *outdata, *indata, *std;


extern char box[6][6];
extern char plus[7][7];
extern char minus[7][7];
extern char sixteen[17][17];
extern char boxmask[6][6];

typedef struct {
    int sw1,sw2;
    int x,y;
    int cenx,ceny;
    } joy_stick;

joy_stick joy;

#define keypressed (bioskey(1) != 0)
#define kb_clear() while keypressed bioskey(0);

/*
main()
{
   int k;

   k = choose_stick(&joy);
   ClrScr();
   if (k != 0) while (!keypressed) {
	read_stick(k,&joy);
	disp_stick(0,&joy);
   }
}

*/





struct savebg
	{
	char far backgnd[256];

	} savedarea[TOTALOBJECTS];



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


void main(void)
{
	int ext=0;

	sp=0;                   // sprite value
	ptext();


	Init_Mode();
	Init_Screen();
	initarray();
	PutColor();
	DrawGrid();

	BoxImage();

	gotoxy (29,23);
	printf("F1 - Help ");


//      if (!Get_Mouse(1))
 //    {
  //        printf("No Mouse Driver Present\n");
   //       exit(1);
    //  }

       //       gotoxy (1,25);         //       Calculate Start of gride 10 lines down
 //     printf("Sprite %d  Row %d ",sp,rw);

      while (ext != 256)

		{
		Get_Mouse(0);
		Calc_Coords();

		Check_Draw();
		Check_Color();
		gotoxy (4,3);
		printf("Sp:%d",sp);

		gotoxy (4,4);
		printf("Rw:%d",rw);

		gotoxy (4,5);
		// printf("fg:%d",animobjects[rw].fshp[sp]->flag);
		printf("Max:%d",animobjects[rw].max);

		gotoxy (4,6);
		printf("Colour: %d",j);

		gotoxy (4,7);
		printf("Flag: %d",animobjects[rw].fshp[sp]->flag);

		if (bioskey(1))
		{
			rc = bioskey(0);
			hc = (rc>>8)&0xff;
			lc = rc&0xff;
		  //    printf("\n");
		  //    printf("%d %d \n",hc,lc);

	if (hc == 1 && lc == 27)   // ESC Key & Exit
		{
		ext=256;
		printf("Exit\n");
		}

		if (hc == 59 && lc == 0)        // 'F1' Help Screen
		{

		nextshape(rw,sp);
		Close_Mode();
		helpscreen();
		Init_Mode();
		placeshape(rw,sp);
		Draw_Data_to_Grid(rw,sp);
		PutColor();
		DrawGrid();

		hc=0;lc=0;
		   }

		if (hc == 63 && lc == 0)        // 'F5' Save Row (current)
		{
		SaveRow();

		hc=0;lc=0;
		   }

		if (hc == 62 && lc == 0)        // 'F4' Do Joystick
		{
		dojoy();

		hc=0;lc=0;
		   }



		if (hc == 22 && lc == 117)        // 'U' Undp Last Change
		{


		clearshape();
		UndoLastChange();
		nextshape(rw,sp);
		Draw_Data_to_Grid(rw,sp);
		hc=0;lc=0;
		   }


		if (hc == 30 && lc == 97)        // 'A'Animate
		   {
		   nextshape(rw,sp);
		   Animate();
		   hc=0;lc=0;
		   }



		   if (hc == 34 && lc == 103)        // 'G'Draw Grid
		   {
		DrawGrid();
		hc=0;lc=0;
		   }

		   if (hc == 68 && lc == 0)        // 'F-10' Debug Mode
		   {
		   Debug();
		   hc=0;lc=0;
		   }

		   if (hc == 66 && lc == 0)        // 'F-8' Layout Mode
		   {
		   nextshape(rw,sp);
		   layout_screen(rw,sp);
		   hc=0;lc=0;
		   }


		   if (hc == 46 && lc == 99)        // 'C'Copy Shape
		   {
		   copyshape();
		hc=0;lc=0;
		   }

		   if (hc == 18 && lc == 101)        // 'E'Erase Shape
		   {
		   EraseObject();
		   hc=0;lc=0;
		   }

		   if (hc == 33 && lc == 102)        // 'F'Flip Shape
		   {
		   nextshape(rw,sp);
		   flipshape();
		   nextshape(rw,sp);
		   Draw_Data_to_Grid(rw,sp);
		   hc=0;lc=0;
		   }

		   if (hc == 50 && lc == 109)        // 'M'Move Shape
		   {
		    DrawGrid();
		     hc=0;lc=0;
		   }







			if (hc == 73 && lc == 0 && rw >0)        // Page Up
			{

				nextshape(rw,sp);
				clearshape();
				cleargrid();

				if (rw >0)
				{

				sp=0;
				rw = rw - 1;
				gotoxy (10,23);
			      //        printf("Row %d ",rw);
			       //       gotoxy (1,25);         //       Calculate Start of gride 10 lines down
			       //       printf("Sprite %d ",sp);
				placeshape(rw,sp);
				Draw_Data_to_Grid(rw,sp);


				hc=0;lc=0;
				}
			}

			if (hc == 81 && lc == 0 && rw < 9)        // Page Down
			{

				nextshape(rw,sp);
				clearshape();
				cleargrid();

				if (animobjects[rw].fshp[sp]->flag==1 && chkdrw ==1 )
				{
				animobjects[rw].max+=1;
				chkdrw=0;
				}

				//if (animobjects[rw].fshp[sp]->flag!=1)



				if (rw < (TOTALANIMS-1))
				{
				rw = rw + 1;
				sp=0;

			      //        gotoxy (10,25);
			      //        printf("Row %d ",rw);
			      //        gotoxy (1,25);         //       Calculate Start of gride 10 lines down
			     // printf("Sprite %d ",sp);

				placeshape(rw,sp);
				Draw_Data_to_Grid(rw,sp);

				hc=0;lc=0;
				}
			}

			if (hc == 47 && lc == 118)        // 'V'View Shapes
			{
			nextshape(rw,sp);
			draw_screen(rw,sp);
			hc=0;lc=0;

			}




			if (hc == 31 && lc == 115)        // 'S'Save Shapes
			{
		      //        gotoxy (1,25);
		      //        printf("Saving ");

			nextshape(rw,sp);
			if (animobjects[rw].max>0)
			{
			filewrite();
			placeshape(rw,sp);
			Draw_Data_to_Grid(rw,sp);
			}
			else
			{
			gotoxy (1,23);
			printf("Nothing to Save ");
			}

			hc=0;lc=0;

			}

			if (hc == 38 && lc == 108)        // 'L'Load Shapes
			{
			sp=0;
			gotoxy (1,23);
			printf("Loading ");
			fileread();
			placeshape(rw,sp);
			Draw_Data_to_Grid(rw,sp);

			hc=0;lc=0;
			}




			if (hc == 74 && lc == 45 && sp >0)        // '-' Moved down 1 object
			{
			 hc=0;lc=0;

			 nextshape(rw,sp);

			 if (sp >0)
			 {
			 sp = sp - 1;

			 clearshape();
			 cleargrid();
			 placeshape(rw,sp);
			 Draw_Data_to_Grid(rw,sp);

		 //      gotoxy (1,25);         //       Calculate Start of gride 10 lines down
		       //        printf("Sprite %d ",sp);
			 }


			}


				if (hc == 78 && lc == 43 && sp <9)        // + Moved over 1 object
				{
				hc=0;lc=0;

				nextshape(rw,sp);

			//      if (animobjects[rw].fshp[sp]->flag==1 && chkdrw ==1 )
			  //    {
			   //   animobjects[rw].max+=1;
			    //  chkdrw=0;
			     // }


				if (sp <10)
				{
				sp = sp + 1;

				clearshape();
				cleargrid();
				placeshape(rw,sp);
				Draw_Data_to_Grid(rw,sp);

				}

			     // gotoxy (1,25);         //       Calculate Start of gride 10 lines down
			   //   printf("Sprite %d ",sp);


		}


		hc=0;lc=0;

		}

	}
	farfree(st1);
//      farfree(undoarea);
	farfree(savearea);


	Close_Mode();

}

void Calc_Coords(void)
{


	   xm/=2;
	   mspeed=2;
	   xm = xm / sval;
	   ym = ym / sval;
	   acxm = xm;                   // Coords for little box
	   acym = ym;
	   xm = xm * sval+1;
	   ym = ym * sval;

	   ny = ym * 320;
	   val = xm + ny;
	   oval = val;

	   tim = oval;

//         gotoxy (1,25);         //       Calculate Start of gride 10 lines down
//         printf("%d %d",oxm,xm);



}

void Init_Screen(void)
{

	if ((savearea = (char far  *) farmalloc(350)) == NULL)
	{
	printf("Not enough memory to allocate buffer\n");
	exit(1);  /* terminate program if out of memory */
	}


}

void initarray(void)
{

	int i,j,s;
	char far *p;
	char far *q;

	if ((p=(char far *)farmalloc(sizeof (struct fshape)*TOTALSHAPE*TOTALANIMS))==NULL)
	{
		printf("Could Not Allocate Memory for Objects\n");
		exit(1);
	}

	_fmemset(p,0,sizeof(struct fshape)*TOTALSHAPE*TOTALANIMS);

	st1 = p;

	if ((q=(char far *)farmalloc(sizeof (struct savebg)*TOTALOBJECTS))==NULL)
	{
		printf("Could Not Allocate Memory for Save Area Objects\n");
		exit(1);
	}




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

	// anims[i].fshp[j]->w=15;
}


void PutColor()
{
	int ad,j,n,v;
	v=0;
	n=0;
	ad=0;
	for (j=0;j<256;j++)
	{
	   for (x=0;x<4;x++)
	   {
		for (y=0;y<4;y++)
		{
		  *(screen+y+ad+offset+n+321)=j;
		}
		offset = offset + 320;      //bytes to next line
	   }
	 //  getch();
	 ad=ad + 5;                     // distance between colors

	 if (ad > 314)                  // if at end go to next line
	 {

		v = v + 5;
		ad = 0;
		n = 320 * v;


	 }
	 offset =0;

	 }
}

void BoxImage()

{



	for (x=0;x<6;x++)
	{
		for (y=0;y<6;y++)
		{
		    *(screen+y+offset+val)=box[y][x];
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
	oy=width;
}

/*
void BoxMask()

{

	for (x=0;x<6;x++)
	{
		for (y=0;y<6;y++)

		{
		  *(screen+y+offset+oldval)^=boxmask[y][x];
		}
		offset = offset + 320;      //bytes to next line
	}

	offset=0;
}
*/

int Get_Mouse(int command)
{
      int found=0;

      if (command)
      {

	      asm     {
		      push    ds
		      mov     ax,0
		      int     0x33
		      mov     [found],ax
		      pop     ds
		      }

		  return found;
	  }

	asm     {
		push    ds
		mov     ax,3
		mov     bx,3
		int     0x33
		mov     [ym],dx
		mov     [xm],cx
		and     bx,7                        // if 0 then no button
		mov     [button],bx                 // if 1 the left button
		pop     ds
		}

	return 0;
}

void CutImage(void)
{
	int i=0;

	for (x=0;x<18;x++)
	{
		for (y=0;y<18;y++)
		{
			*(savearea+i)=*(screen+y+offset+val);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}

	offset=0;
}

void PutImage(void)
{

	for (x=0;x<4;x++)
	{
		for (y=0;y<4;y++)
		{
		  *(screen+y+offset+val)=j;
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;

}

void RestoreBack(void)
{
	int i=0;

	for (x=0;x<18;x++)
	{
		for (y=0;y<18;y++)
		{
			 *(screen+y+offset+val)=*(savearea+i);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
}





void Check_Color(void)
{
	// Colors are divisable by 2 since they are 4 wide
	// and 4 high
	// start at 0
	// increment by 5
	int cl=320*2+2;



	if (ym <= 18 && button==1)
	 {
	j = *(screen +val+cl);          // turn the block in to the color
					// 'J' is the color
	for (x=0;x<6;x++)               // draw the boax around the color
	{
		for (y=0;y<6;y++)
		{
		    *(screen+y+offset+val-1)^=box[y][x];
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
	}

}
void DrawGrid(void)
{

	st = 64 * 320+50;
	for (c=0;c<17;c++)
	{
		for (b=0;b<80;b++)             // 60 Lines high and 60 accross
		{
		*(screen+b+offset+st)^=23;
		}
		offset = offset + 320*5;
	}
	offset=0;
	st=64 * 320+50;                 //start location

	for (c=0;c<17;c++)
	{

		for (b=0;b<80;b++)             // 60 Lines high and 60 accross
		{
		*(screen+b+offset+st+step)^=23;
		offset = offset + 319;
		}
		offset = 0;
		step = step + 5;
	}
	offset=0;
	step=0;

}

void Colors(void)
{
	long int b;

	 for (c=16;c<32;c++)
	 {
		for (b=0;b<64000;b++)
		{
		*(screen+b)=c;
		}
	 }
}
void Check_Draw(void)
{


		if (button==0)
		{

			CutImage();
			for (tm=0;tm<1100;tm++)
			if (layout==0)
				{
				BoxImage();
				}
			else
			{
			BigBox(rw,sp);
			}


		       //       PutImage();
			SyncToVerticalRetrace();
			RestoreBack();
			lso=0;
		 //     if(lso == 0 && chkdrw ==1)
		   //           SaveLastImage();



		}
		if (button==1)

		{

			if (xm >=ga && ym >=gb && ym <= gc && xm <=gd)
			{
				if (animobjects[rw].fshp[sp]->flag==0)
					animobjects[rw].max+=1;
				animobjects[rw].fshp[sp]->flag=1;
			      //
				allflg=1;
				chkdrw=1;
				if (lso==0)
				{
					SaveLastImage();
					lso=1;
				}

				//lso=0;
			       //       if (chgpnt==1 && ym >= 140 && xm >= 220)
			      //                DrawPnt();
			      //        else
			      //        {

				if (layout==1)
				{
					BigBox(rw,sp);
				}
				else
				{
				PutImage();
				*(screen + acxm + acym * 320 + 32260) = j;
				}
			}

			    //  }

		}
	     // oxm = xm;


}



void drawmem(void)
{

	int b,c,loc;

	loc = 320*100;
	offset=0;
	for (b=0;b<6;b++)
	{
		for(c=0;c<6;c++)
		{
		if (box[b][c]>0)
			{
			*(screen+c+offset+loc)^=box[b][c];
			*(screen+c+offset+320+32260) = box[b][c];
			}

//                     getch();
		}
		offset=offset+320;
	}
	offset=0;

	getch();

}


void Draw_Data_to_Grid(int rw , int sp)

{

	int blkclr,gx,gy,cx,cy,incr=0,dninc=0;

	st = 65 * 320+51;
	dninc = st;

	for (gx=0;gx<16;gx++)
	{
	    for (gy=0;gy<16;gy++)
	    {
		for (cy=0;cy<4;cy++)
		{
			for (cx=0;cx<4;cx++)             // 60 Lines high and 60 accross
			{
			*(screen+cx+st+offset)=animobjects[rw].fshp[sp]->shp[gx*16+gy];

			}
			offset = offset + 320;
		}
		st = st +5;
		offset=0;
	    }
	    incr = incr + 5;
	    st = dninc + (incr*320);

	}
}


void nextshape(int rw , int sp)
{

	int i,j,vz;

	vz=0;

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{
			animobjects[rw].fshp[sp]->shp[i*16+j]=*(screen+j+36430+vz);

		}
		vz = vz + 320;
	       //       getch();
	    }

//      printf("Done next Shape\m");

}


void drawshape(int rw , int sp)
{

	int i,j,vz;

	vz=0;

	for (i=0;i<x;i++)

	    {
		for (j=0;j<y;j++)
		{

			*(screen+j+vz)=animobjects[rw].fshp[sp]->shp[i*16+j];

		}

		vz = vz + 320;

	    }




}


void clearshape(void)
{

	int i,j,vz;

	vz=0;

	for (i=0;i<16;i++)
	    {
		for (j=0;j<16;j++)
		{

			*(screen+j+36430+vz)=0;

		}
		vz = vz + 320;
	    }



}

void cleargrid(void)
{

	int blkclr,gx,gy,cx,cy,incr=0,dninc=0;


	st = 65 * 320+51;

	dninc = st;

	offset=0;

	for (gx=0;gx<16;gx++)
	{
	    for (gy=0;gy<16;gy++)
	    {
		for (cy=0;cy<4;cy++)
		{
			for (cx=0;cx<4;cx++)             // 60 Lines high and 60 accross
			{
			*(screen+cx+st+offset)=0;
			}
			offset = offset + 320;
		}
		st = st +5;
		offset=0;
	    }
	    incr = incr + 5;
	    st = dninc + (incr*320);

	}




}

void placeshape(int rw, int sp)
{

	int i,j,vz;

	vz=0;

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{

			*(screen+j+36430+vz)=animobjects[rw].fshp[sp]->shp[i*16+j];

		}
		vz = vz + 320;

	    }


}


int filewrite(void)
{

   int i,numobj,numrow;
   char label[20];
   char infst[20];
   char stan[20];

   gotoxy (1,23);
   printf("Please enter Filename: ");
   fflush(stdin);  /* flush the input stream in case of bad input */

   scanf("%8s",label);
   strcpy(infst, label);
   strcpy(stan, label);

   strcat(label,".dat");
   strcat(infst,".inf");
   strcat(stan,".std");


   if ((std = fopen(stan, "wb"))
	   == NULL)
   {
	  fprintf(stderr, "Cannot open Standard file.\n");
	  return 1;
   }


   if ((out = fopen(infst, "wb"))
	   == NULL)
   {
	  fprintf(stderr, "Cannot open output file.\n");
	  return 1;
   }

      //        if ((outdata = fopen(label, "wb"))
	//         == NULL)
	  //    {
	    //    fprintf(stderr, "Cannot open output file.\n");
	    //    return 1;
	     // }

	       //       fwrite(animobjects,sizeof(animobjects),1,outdata);


	for (numrow=0;numrow<10;numrow++)
	{
		fprintf(out,"%d\n",animobjects[numrow].max);
		for (numobj=0;numobj<10;numobj++)
		{
			if (animobjects[numrow].fshp[numobj]->flag==1)
			{

			  fwrite(animobjects[numrow].fshp[numobj],272l,1,out);
			  fwrite(animobjects[numrow].fshp[numobj]->shp,256,1,std);
			}
		}
	}

   fclose(out);
  // fclose(outdata);
   fclose(std);
   return 1;

}

int fileread(void)
{

	int i,j,numobj,numrow;
	char far *p;
	char label[20];
	char infst[20];

	gotoxy (1,23);
	printf("Please enter Filename: ");
	fflush(stdin);  /* flush the input stream in case of bad input */

	scanf("%8s",label);

	strcpy(infst, label);

	strcat(label,".dat");
	strcat(infst,".inf");


	if ((in = fopen(infst, "rb"))
	   == NULL)
	{
	  fprintf(stderr, "Cannot open input file.\n");
	  return 1;
	}

   //   if ((indata = fopen(label, "rb"))
   //              == NULL)
    //          {
 //               fprintf(stderr, "Cannot open output file.\n");
   //             return 1;
    //          }

	//      fread(animobjects,sizeof(animobjects),1,indata);


      //        p = st1;

     // for (i=0;i<10;i++)
     // {

     //         for (j=0;j<10;j++)

		     // {
		     // animobjects[i].fshp[j]=(struct fshape *)p;
		     // p+=sizeof(struct fshape);

		      //        }
       //       }


	for (numrow=0;numrow<10;numrow++)
	{
	    //  printf ("%d\n",animobjects[numrow].max);

	    fscanf(in,"%d",&animobjects[numrow].max);
	      fgetc(in);

	    //  if (animobjects[numrow].max >9)
	    //  animobjects[numrow].max=9;

		for (numobj=0;numobj<=animobjects[numrow].max;numobj++)
	      //        getch();
		{
		fread(animobjects[numrow].fshp[numobj],272l,1,in);
		animobjects[numrow].fshp[numobj]->flag=1;
		}
	}
//      getch();
//        getch();






   fclose(in);
   fclose(indata);


   return 1;



}



void draw_screen(int rw, int sp)
{
	int ap=0;
	int dn=0;
	int cnt=0;
	int t,s,y;
	long int clr;

	for (clr=0;clr<64000;clr++)
		{
		*(screen+clr)=0;
		}

       //       printf("%d %d",rw,sp);

	for (t=0;t<9;t++)
	{
		for (s=0;s<=9;s++)
		{
			if (animobjects[t].fshp[s]->flag==1)
			{
				for (x=0;x<16;x++)
				{
					for (y=0;y<16;y++)
					{
					*(screen+y+offset+ap+dn)=animobjects[t].fshp[s]->shp[x*16+y];
					}
					offset = offset + 320;      //bytes to next line
				}

				offset = 0;
				ap = ap +18;
				//getch();
			}
		}
		cnt = cnt + 18;
		dn = cnt * 320;
		offset=0;
		ap=0;


	}
	gotoxy (1,23);
	printf("Press Any Key to Return");
	getch();
		for (clr=0;clr<64000;clr++)
		{
		*(screen+clr)=0;
		}

	placeshape(rw,sp);
	Draw_Data_to_Grid(rw,sp);
	PutColor();
	DrawGrid();
	gotoxy (29,23);
	printf("F1 - Help ");


}


void copyshape(void)
{

    int cp,dn,i;

    gotoxy (1,23);
    printf("Source :");
    cp=getch()-'0';

    if (cp < 0 || cp > 9)
	printf("Object out of Row Range");
    else
    {
	gotoxy (1,23);
	printf("Destin :");
	dn=getch()-'0';
		if (dn <0 || dn >9)
			{
			gotoxy (1,23);
			printf("Object destination out of Range");
			}
		else
			if (animobjects[rw].fshp[dn]->flag==1)
				{
				gotoxy (1,23);
				printf("Destination Contains Data");
				}
				else
				{
				for (i=0;i<259;i++)
				{
				animobjects[rw].fshp[dn]->shp[i]=animobjects[rw].fshp[cp]->shp[i];
				}
				animobjects[rw].fshp[dn]->flag=1;
				animobjects[rw].max+=1;
				placeshape(rw,sp);
				Draw_Data_to_Grid(rw,sp);

				}

    }





}



void EraseObject(void)
{

	clearshape();
	cleargrid();
	if (animobjects[rw].max>0 && animobjects[rw].fshp[sp]->flag != 0)
		animobjects[rw].max--;
	animobjects[rw].fshp[sp]->flag=0;
	nextshape(rw,sp);


}


void SaveLastImage(void)
{

	// *(savearea+i)=*(screen+y+offset+val);
	int i,j,vz;

	vz=0;

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{
			undoarea[j+i*16]=*(screen+j+36430+vz);

		}
		vz = vz + 320;
	    }

	    lso=1;

}

void UndoLastChange(void)
{

	int i,j,vz;

	vz=0;

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{
			*(screen+j+36430+vz)=undoarea[i*16+j];

		}
		vz = vz + 320;
	    }




}


void flipshape(void)
{

	int i,j,vz,dn;

	vz=0;

	gotoxy (1,23);
	printf("Flip (x or y)`:");

	dn=getch();

	if (dn == 'Y' || dn == 'y')
	{

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{

			*(screen+j+36430+vz)=animobjects[rw].fshp[sp]->shp[(15-i)*16+j];

		}
		vz = vz + 320;

	    }
	}

	if (dn == 'X' || dn == 'x')
	{
	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{

			*(screen+j+36430+vz)=animobjects[rw].fshp[sp]->shp[i*16+15-j];

		}
		vz = vz + 320;

	    }
	}



}


void Animate(void)
{

    int cp,dn,i,spp;
    long int t;

    gotoxy (1,23);
    printf("Start :      ");
    cp=getch()-'0';

    if (cp < 0 || cp > 9)
	printf("Object out of Row Range");
    else
    {
	gotoxy (1,23);
	printf("Destin :          ");
	dn=getch()-'0';

	if (dn <0 || dn >9)
	{
		gotoxy (1,23);
		printf("Object destination out of Range");
	}
	else
	{
		gotoxy (1,23);
		printf("Speed 1 -8:");
		spp=getch()-'0';

		if (spp <0 || spp >8)
		{
		gotoxy (1,23);
		printf("Wrong Speed    ");
		}

		else
		{
		gotoxy (1,23);
		printf("                   ");

		while(!bioskey(1))
		{
		for (i=cp;i<dn;i++)
		    {
		    placeshape(rw,i);
			delay(50*spp);
		}

		    }
		}
		getch();
	}


    }
    placeshape(rw,sp);
    Draw_Data_to_Grid(rw,sp);


}


void CutObject(int rw, int sp)
{
	int i=0;
	int tempx,tempy;

	for (i=0;i<TOTALOBJECTS;i++)
	{
	    tempx = animobjects[rw].animx;
	    tempy = animobjects[rw].animy * 320 ;
	    offset = tempx + tempy;

	    for (x=0;x<16;x++)
	    {
		for (y=0;y<16;y++)
		{
		savedarea[i].backgnd[i*16+j]=*(screen+offset+y);
		}
		offset = offset + 320;      //bytes to next line
	    }

	offset=0;
	}
}

void PutObject(void)
{
	// put objects on screen
	// save object to oldobject coords
	// add speed to object
	// add animation to next frame
	// check if object is at end of anim - if yes set to 0
	int i;

	for (i=0;i<TOTALOBJECTS;i++)
	{
	    for (x=0;x<16;x++)
	    {
		for (y=0;y<16;y++)
		{
		  *(screen+y+offset+val)=animobjects[rw].fshp[sp]->shp[i*16+j];
		}
		offset = offset + 320;      //bytes to next line
	    }
	offset=0;
	}
}

void RestoreObject(void)
{
	int i=0;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
			 *(screen+y+offset+val)=*(savearea+i);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
}


void Debug(void)
{


    int cp;

    gotoxy (1,23);
    printf("Debug Max Objects:      ");
    scanf("%d",&cp);

    if (cp < 0 || cp > 10)
	printf("Object out of Row Range");
    else
    {
	animobjects[rw].max=cp;
    }


}


void layout_screen(int rw, int sp)
{
	int ap=0;
	int dn=0;
	int cnt=0;
	int t,s,y;
	long int clr;
	int rc,hc=0,lc=0;
	int bg=1;
	int cp,i,spp;
	int bn=0;
	int sk;
	int c,d;
	int ext;
	int saveblk;
	int saverw;

	offset=160*320;
	BigBox(rw,sp);

	for (clr=0;clr<64000;clr++)
		{
		*(screen+clr)=0;
		}

	DrawRow(rw);


    ap=0;
    dn=0;
    offset=0;
    ga=0;
    gb=0;
    gc=190;
    gd=250;
    sval=16;    // set step value to 1
    saveblk=sp;
    saverw=rw;
    gotoxy (20,23);
    printf("Right Mouse to Exit");

    draw_plus_minus();

    while (button!=2)
    {
	  //  gotoxy (1,24);
	  //  printf("%d  %d  %d ",xm,ym,sp);


		Get_Mouse(0);
		Calc_Coords();


		layout=1;

		Check_Draw();


		Check_Select();





     }
	gotoxy (1,23);
	printf("Press Any Key to Return");
	getch();

		for (clr=0;clr<64000;clr++)
		{
		*(screen+clr)=0;
		}

	placeshape(rw,sp);
	Draw_Data_to_Grid(rw,sp);
	PutColor();
	DrawGrid();
	gotoxy (29,23);
	printf("F1 - Help ");

	layout=0;
	ga=47;
	gb=61;
	gc=144;
	gd=126;
	sval=5;
	sp=saveblk;         //  Reset
	rw=saverw=rw;


}

void DrawRow(int rw)
{

	int x;
	int ap=0;
	int dn=0;
	int cnt=0;
	int s,v;


	offset=160*320;


		for (s=0;s<=9;s++)
		{
			if (animobjects[rw].fshp[s]->flag==1)
			{
				for (x=0;x<16;x++)
				{
					for (y=0;y<16;y++)
					{
					*(screen+y+offset+ap+dn)=animobjects[rw].fshp[s]->shp[x*16+y];
					}
					offset = offset + 320;      //bytes to next line
				}

				offset = 160*320;
				ap = ap +18;
				//getch();
			}
		}
		cnt = cnt + 18;
		dn = cnt * 320;
		offset=0;
		ap=0;






}

void BigBox(int rw, int sp)
{

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
		    //if (layout==1)
		   // *(screen+y+offset+val)^=sixteen[x][y];
		   // else
		  //  {
		    *(screen+y+offset+val)=animobjects[rw].fshp[sp]->shp[x*16+y];;
		  //  }


		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
	oy=width;



}







void Check_Select(void)
{
	int ext;

	if (bioskey(1))
	{
		rc = bioskey(0);
		hc = (rc>>8)&0xff;
		lc = rc&0xff;
		  //    printf("\n");
		  //    printf("%d %d \n",hc,lc);

		gotoxy (20,23);
		printf("SP= %d RW= %d",sp,rw);

		if (hc == 74 && lc == 45)   // - Key
		{
			if (sp <=0 && rw >0)
			{
			sp=9;
			rw = rw -1;
			DrawRow(rw);
			}
			else
			{
			if (sp >0)
			{
			sp = sp -1;
			hc=0; lc=0;
			}
			}

		}

		if (hc == 78 && lc == 43)        // '+'
		{
			if (sp >8 && rw <=9)
			{
			sp=0;
			rw = rw + 1;
			DrawRow(rw);
			}
			else
			{
			sp = sp + 1;
			hc=0;lc=0;
			}
		}


	}

}


void draw_plus_minus(void)

{

	int i,j,vz;

	vz=0;

	for (i=0;i<7;i++)
		{
		for (j=0;j<7;j++)
		{

			*(screen+j+57600+vz)=minus[i][j];
			*(screen+j+57610+vz)=plus[i][j];

		}
		vz = vz + 320;

	    }


}

void DrawPnt(void)
{


	*(screen+y+offset+val)=0x0b;






}

void SaveRow(void)
{

 //   struct savebg
 //     {
 //     char far backgnd[256];
 //
 //     } savedarea[TOTALOBJECTS];



    int cp,dn,i,v;


    if (memflag==0)
    {
	gotoxy (1,23);
	printf("Source :");
	cp=getch()-'0';



	if (cp < 0 || cp > 9)
		printf("Object out of Row Range");


	{
		for (v=0;v<10;v++)
		{
		      for (i=0;i<=256;i++)
		      {
		      savedarea[v].backgnd[i]=animobjects[cp].fshp[v]->shp[i];
		      }
		}
		memflag=1;

	}
    }
    else
    {
    memflag=0;
    gotoxy (1,23);
	printf("Destin :");
	dn=getch()-'0';
		if (dn <0 || dn >9)
		{
			gotoxy (1,23);
			printf("Object destination out of Range");
		}

	      //        if (animobjects[rw].fshp[dn]->flag==1)
	      //        {
	      //                gotoxy (1,25);
	      //                printf("Destination Contains Data");
	       //       }
		else
			{


			 for (v=0;v<10;v++)
			{
			    for (i=0;i<=256;i++)
			    {
			     animobjects[dn].fshp[v]->shp[i]=savedarea[v].backgnd[i];
			    }
			    animobjects[dn].fshp[v]->flag=1;
			    //printf("%d ",animobjects[dn].fshp[v]->flag);
			   // getch();
			 }
			memflag=0;
			}

     }
}

void GotoXY(int x,int y)
{
    union REGS r;
/* Set XY position */
    r.h.ah = 2;
    r.h.bh = 0;            /* Assume Video Page 0 */
    r.h.dh = (char) y;
    r.h.dl = (char) x;
    int86(16,&r,&r);
}

void ClrScr()
{
    union REGS r;

/* Get video mode */
    r.h.ah = 15;
    int86(16,&r,&r);

/* Set video mode */
    r.h.ah = 0;
    int86(16,&r,&r);
}

void disp_stick(int line,joy_stick *joy)
{
    GotoXY(5,20);
    printf("sw1 %d sw2 %d",joy -> sw1,joy -> sw2);
    GotoXY(5,21);
    printf("x %4d y %4d",joy -> x,joy -> y);
}

void read_stick(int stick,joy_stick *joy)
{
    if (stick == 1) m4 = 1; else
    if (stick == 2) m4 = 4; else
    printf("Invalid stick %d\n",stick);

    m5 = m4 << 1;
    m1 = m4 << 4;
    m2 = m5 << 4;
    m3 = m4 + m5;

/* Trigger joystick */
    outportb(0x201,0xff);
    c = inportb(0x201);

/* Read switch settings */
    joy -> sw1 = (c & m1) == 0;
    joy -> sw2 = (c & m2) == 0;

/* Get X and Y positions */
    for (k = 0; (c & m3) != 0; k++) {
	if ((c & m4) != 0) jx = k;
	if ((c & m5) != 0) jy = k;
	c = inportb(0x201);
    }
    joy -> x = jx - (joy -> cenx);
    joy -> y = jy - (joy -> ceny);
}

int choose_stick(joy_stick *joy)
{
    int init_swa,init_swb,swa,swb;
    int c,retval;

    printf("Center joystick and press fire, or press any key\n");
    kb_clear();
    outportb(0x201,0xff);
    c = inportb(0x201);
    init_swa = c & 0x30;
    init_swb = c & 0xc0;
    do {
       outportb(0x201,0xff);
       c = inportb(0x201);
       swa = c & 0x30;
       swb = c & 0xc0;
    } while ((swa == init_swa) && (swb == init_swb) && !keypressed);
    if (swa != init_swa) {
       printf("Joystick 1 selected\n");
       retval = 1;
    } else if (swb != init_swb) {
       printf("Joystick 2 selected\n");
       retval = 2;
    } else {
       printf("Keyboard selected\n");
       kb_clear();
       retval = 0;
    }

    if (retval != 0) { /* Determine Center */
       joy -> cenx = joy -> ceny = 0;
       read_stick(retval,joy);
       joy -> cenx = joy -> x;
       joy -> ceny = joy -> y;
    }

    return(retval);
}


/*
void ClearAllData(void)
{
	int i,j;
//      for (i=0;i<10;i++)
		for (j=0;j<10;j++)
 //                   animobjects[j]=0; 0



}

  */


void dojoy(void)
{

   Close_Mode();

   k = choose_stick(&joy);

   if (k != 0) while (!keypressed) {
	read_stick(k,&joy);
	disp_stick(0,&joy);
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

