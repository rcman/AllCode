// Drawing Program
// Date Start: June 1, 1995
// Revision: April 4 - 1996


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
#define TOTALSHAPE 10
#define TOTALANIMS 10

void Init_Screen(void);
void initarray(void);
void CutImage(void);
void PutImage(void);
void RestoreBack(void);
void PutColor(void);
void BoxImage(void);
void BoxMask(void);
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
int filewrite(void);
int fileread(void);
void EraseObject(void);
void SaveLastImage(void);
void UndoLastChange(void);


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
char far *undoarea;
int nx,ny,val,mspeed,oldoffset,oldy,oldval,tm,oval,ptflag;
int c,b,st,ac,step,tim;
int inc=5;
int lso=0;
int acxm,acym;          // Actuall Y Mouse and X mouse
char far *point;
char *p;
char far *st1;

int sp,rw=0;

char sc[20];


FILE *in, *out, *outdata, *indata;


static char box[6][6] = {
    { 0x0a, 0x0a, 0x0a, 0x0a,0x0a, 0x0a, },
    { 0x0a, 0x00, 0x00, 0x00,0x00, 0x0a, },
    { 0x0a, 0x00, 0x00, 0x00,0x00, 0x0a, },
    { 0x0a, 0x00, 0x00, 0x00,0x00, 0x0a, },
    { 0x0a, 0x00, 0x00, 0x00,0x00, 0x0a, },
    { 0x0a, 0x0a, 0x0a, 0x0a,0x0a, 0x0a, },
      };
static char boxmask[6][6] = {
    { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, },
    { 0x10, 0x00, 0x00, 0x00, 0x00, 0x10, },
    { 0x10, 0x00, 0x00, 0x00, 0x00, 0x10, },
    { 0x10, 0x00, 0x00, 0x00, 0x00, 0x10, },
    { 0x10, 0x00, 0x00, 0x00, 0x00, 0x10, },
    { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, },

      };


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
	gotoxy (29,25);
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



		gotoxy (1,25);

		if (bioskey(1))
		{
			rc = bioskey(0);
			hc = (rc>>8)&0xff;
			lc = rc&0xff;

		       //	printf("%d %d \n",hc,lc);

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

		if (hc ==22 && lc == 117)        // 'U' Undp Last Change
		{


		clearshape();
                getch();
		UndoLastChange();
		//nextshape(rw,sp);
		//Draw_Data_to_Grid(rw,sp);
		hc=0;lc=0;
		   }



		   if (hc == 34 && lc == 103)        // 'G'Draw Grid
		   {
		DrawGrid();
		hc=0;lc=0;
		   }


		   if (hc == 46 && lc == 99)        // 'C'Copy Shape
		   {
		DrawGrid();
		hc=0;lc=0;
		   }

		   if (hc == 18 && lc == 101)        // 'E'Erase Shape
		   {
		EraseObject();
		hc=0;lc=0;
		   }

		   if (hc == 33 && lc == 102)        // 'F'Flip Shape
		   {
		DrawGrid();
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
				gotoxy (10,25);
				printf("Row %d ",rw);
				gotoxy (1,25);         //       Calculate Start of gride 10 lines down
				printf("Sprite %d ",sp);
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


				if (rw < (TOTALANIMS-1))
				{
				rw = rw + 1;
				sp=0;

				gotoxy (10,25);
				printf("Row %d ",rw);
				gotoxy (1,25);         //       Calculate Start of gride 10 lines down
				printf("Sprite %d ",sp);

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
			gotoxy (1,25);
			printf("Saving ");

			nextshape(rw,sp);
			if (chkdrw==1)
			{
			filewrite();
			placeshape(rw,sp);
			Draw_Data_to_Grid(rw,sp);
			}
			else
			{
			gotoxy (1,25);
			printf("Nothing to Save ");
			}

			hc=0;lc=0;

			}

			if (hc == 38 && lc == 108)        // 'L'Load Shapes
			{
			sp=0;
			gotoxy (1,25);
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

			 gotoxy (1,25);         //       Calculate Start of gride 10 lines down
			 printf("Sprite %d ",sp);
			 }


			}


				if (hc == 78 && lc == 43 && sp <9)        // + Moved over 1 object
				{
				hc=0;lc=0;

				nextshape(rw,sp);
				animobjects[rw].max+=1;


				if (sp <10)
				{
				sp = sp + 1;

				clearshape();
				cleargrid();
				placeshape(rw,sp);
				Draw_Data_to_Grid(rw,sp);

				}

				gotoxy (1,25);         //       Calculate Start of gride 10 lines down
				printf("Sprite %d ",sp);


		}


		hc=0;lc=0;

		}

	}
	farfree(st1);
	farfree(undoarea);
	farfree(savearea);


	Close_Mode();

}





void Calc_Coords(void)
{
	   int sval=5;
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

	   gotoxy (1,25);         //       Calculate Start of gride 10 lines down
//         printf("%d %d",oxm,xm);



}

void Init_Screen(void)
{

	if ((savearea = (char far  *) farmalloc(350)) == NULL)
	{
	printf("Not enough memory to allocate buffer\n");
	exit(1);  /* terminate program if out of memory */
	}

	if ((undoarea = (char far  *) farmalloc(350)) == NULL)
	{
	printf("Not enough memory to allocate undo buffer\n");
	exit(1);  /* terminate program if out of memory */
	}


}

void initarray(void)
{

	int i,j,s;
	char far *p;

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
		    *(screen+y+offset+val)^=box[y][x];
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
	oy=width;
}

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

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
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

void Check_Color(void)
{
	// Colors are divisable by 2 since they are 4 wide
	// and 4 high
	// start at 0
	// increment by 5
	int cl=320*2+2;



	if (ym <= 12 && button==1)
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
		BoxImage();
		PutImage();

		RestoreBack();

		if(lso == 0 && chkdrw ==1)
			SaveLastImage();

		}

			if (button==1)


			{

				if (xm >=47 && ym >=61 && ym <= 144 && xm <=126)

				 {
				animobjects[rw].fshp[sp]->flag=1;
				animobjects[rw].max=1;
				chkdrw=1;

				lso=0;

				PutImage();
				*(screen + acxm + acym * 320 + 32260) = j;
				}



			}

//                      oxm = xm;
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


   if ((out = fopen("fileout.dat", "wb"))
	   == NULL)
   {
	  fprintf(stderr, "Cannot open output file.\n");
	  return 1;
   }

	if ((outdata = fopen("filedata.dat", "wb"))
		   == NULL)
		{
		  fprintf(stderr, "Cannot open output file.\n");
		  return 1;
		}
		fwrite(animobjects,sizeof(animobjects),1,outdata);


	for (numrow=0;numrow<10;numrow++)
	{
		for (numobj=0;numobj<=animobjects[numrow].max;numobj++)
		{
			if (animobjects[numrow].fshp[numobj]->flag==1)
			{

			  fwrite(animobjects[numrow].fshp[numobj],272l,1,out);

			}
		}
	}

   fclose(out);
   fclose(outdata);
   return 1;

}

int fileread(void)
{

	int i,j,numobj,numrow;
	char far *p;

   if ((in = fopen("fileout.dat", "rb"))
	   == NULL)
   {
	  fprintf(stderr, "Cannot open input file.\n");
	  return 1;
   }

	if ((indata = fopen("filedata.dat", "rb"))
		   == NULL)
		{
		  fprintf(stderr, "Cannot open output file.\n");
		  return 1;
		}

		fread(animobjects,sizeof(animobjects),1,indata);

	p = st1;

	for (i=0;i<TOTALANIMS;i++)
	{
		for (j=0;j<TOTALSHAPE;j++)

			{
			animobjects[i].fshp[j]=(struct fshape *)p;
			p+=sizeof(struct fshape);

			}
	}


	for (numrow=0;numrow<10;numrow++)
	{
		for (numobj=0;numobj<=animobjects[numrow].max;numobj++)
		{

		fread(animobjects[numrow].fshp[numobj],272l,1,in);
		}
	}





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

	for (t=0;t<10;t++)
	{
		for (s=0;s<=animobjects[t].max;s++)
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
				ap = ap +16;
				//getch();
		}
		cnt = cnt + 18;
		dn = cnt * 320;
		offset=0;
		ap=0;


	}
	gotoxy (1,25);
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
	gotoxy (29,25);
	printf("F1 - Help ");


}


void copyshape(void)
{





}



void EraseObject(void)
{

	clearshape();
	cleargrid();
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
			*(undoarea+i*j)=*(screen+j+36430+vz);

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
			*(screen+j+36430+vz)=*(undoarea+i*j);

		}
		vz = vz + 320;
	    }




}

