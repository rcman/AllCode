/*
// GAMDEMO.C
//
// Game port demo program for the Gport game port library.
//
// Compiled and linked with Borland C++ v2.00
//
// Copyright (c) 1991 Bri Productions
//
*/


#include "gport.h"
#include <stdlib.h>
#include <bios.h>
#include <graphics.h>
#include <alloc.h>
#include <stdio.h>
#include <stdarg.h>
#include <dos.h>


#define  AXIS  0                    /* GamAxis()/GamStick switch  */

#define  limit(n,max,min)  (n) > (max) ? (max) : ((n) < (min) ? (min) :(n))


/*
//-------------------------------------
//
// screen coordinates, sizes, etc.
//
//-------------------------------------
*/

#define  BASE_X      640            /* base with of the screen */
#define  BASE_Y      480            /* base with of the screen */
#define  XREF        9              /* X scaling reference     */
#define  YREF        35             /* X scaling reference     */
#define  OAX         170            /* x origin of joystick a  */
#define  OAY         175            /* y origin of joystick a  */
#define  OBX         470            /* x origin of joystick b  */
#define  OBY         175            /* y origin of joystick b  */
#define  JR          135            /* joystick radius         */
#define  TSIZE       5              /* size of tic marks       */
#define  TSPACE      10             /* spacing of tic marks    */
#define  TNUM        13             /* number of tic marks     */
#define  HSIZE       5              /* cross hair size         */
#define  BAY         350            /* y center of a buttons   */
#define  BBY         350            /* y center of b buttons   */
#define  BSPACE      100            /* x space between buttons */
#define  BR          20             /* button radius           */
#define  BON         1              /* button on               */
#define  BOFF        0              /* button off              */
#define  GAIN        7              /* gain divisor for Gport  */
#define  BIOS_GAIN   14             /* gain multiplier for bios*/

#define  Y_COORD_OFF 235            /* Y coordinate offset     */
#define  JAY_COORD   (OAY) + Y_COORD_OFF
#define  JBY_COORD   (OAY) + Y_COORD_OFF
#define  TEXT_Y   370               /* text y position         */


/*
//-------------------------------------
//
// Text justification
//
//-------------------------------------
*/

#define  J_LEFT      0              /* left justified text     */
#define  J_CENTER    1              /* center justified text   */
#define  J_RIGHT     2              /* right justified text    */

/*
//-------------------------------------
//
// keystrokes
//
//-------------------------------------
*/

#define  F1    0x3B00
#define  F2    0x3C00
#define  F3    0x3D00
#define  F4    0x3E00
#define  F5    0x3F00
#define  F6    0x4000
#define  F7    0x4100
#define  F8    0x4200
#define  F9    0x4300
#define  F10   0x4400
#define  ESC   0x011B


/*
//-------------------------------------
//
// function prototypes
//
//-------------------------------------
*/

void  Init        (void);
void  Uninit      (void);
void  PutAxis     (int x, int y);
void  PutButton   (int x, int off);
void  CrossHair   (int x, int y);
void  Button      (byte status);
void  PutCoord    (int x, int y, int just, signed value);
void  gputs       (int x, int y, int color, int just, char *str);
void  gerase      (int x, int y, int color, int just, char *str);


/*
//-------------------------------------
//
// global variables
//
//-------------------------------------
*/

void  *old_image[2];             /* image behind the cross hair   */
int   Xasp, Yasp;                /* X and Y aspect ratios         */
int   bios_ax0, bios_ay0;        /* bios values for joystick A    */
int   bios_bx0, bios_by0;        /* bios values for joystick B    */


/*
//-------------------------------------
//
// default colors (vga/ega)
//
//-------------------------------------
*/

int   color   = YELLOW;          /* default color     */
int   bcolor  = RED;             /* button color      */
int   vcolor  = YELLOW;          /* values color      */
int   tcolor  = GREEN;           /* text color        */
int   hcolor  = WHITE;           /* cross hair color  */
int   bkcolor = DARKGRAY;        /* background color  */


/*
//-------------------------------------
//
// function macros
//
//-------------------------------------
*/

#define  X(a)  (a)*XREF/Xasp     /* X scaling   */
#define  Y(a)  (a)*YREF/Yasp     /* Y scaling   */


/*
//-------------------------------------
//
// main()
//
//-------------------------------------
*/

void main(void)
{
signed ax, ay, bx, by;                 /* joystick coordinates       */
signed old_ax, old_ay, old_bx, old_by; /* old joystick coordinates   */
byte   bstat;                          /* button status              */
byte   old_bstat = 0;                  /* old button status          */
int    key = 0;                        /* keystroke                  */
int    gport = 1;                      /* gport mode flag            */
union  REGS regs;                      /* CPU registers for bios     */
char   *mode[] = { "Bios", "Gport" };  /* mode text                  */
char   *cmode[] = { "Real", "Mean" };
byte   center = 0;


   Init();
   old_ax = old_ay = old_bx = old_by =0x7fff;


   while(key != ESC)
   {

         /* If we are not in Gport mode, call the bios to */
         /* get the joystick positions.                   */

      if(!gport)
      {
         regs.h.ah = 0x84;
         regs.x.dx = 1;
         int86(0x15, &regs, &regs);
         ax = ( regs.x.ax - bios_ax0) * BIOS_GAIN;
         ay = (-regs.x.bx + bios_ay0) * BIOS_GAIN;
         bx = ( regs.x.cx - bios_bx0) * BIOS_GAIN;
         by = (-regs.x.dx + bios_by0) * BIOS_GAIN;

         ax = limit(ax,1000,-1000);
         ay = limit(ay,1000,-1000);
         bx = limit(bx,1000,-1000);
         by = limit(by,1000,-1000);

      }


         /* If we are in Gport mode, call GamAxis or GamStick to */
         /* get the joystick position for joystick A             */

      if(gport)
      {
#if AXIS
         ax = GamAxis(JAX);
         ay = GamAxis(JAY);
#else
         GamAxes(JA, &ax, &ay);
#endif
      }


         /* If either coordinate has changed since the last time  */
         /* it was checked, the screen must be updated with the   */
         /* new value                                             */

      if(ax != old_ax)
      {
         PutCoord(OAX, JAY_COORD, textwidth("  +0000"), ax);
         old_ax = ax;
      }
      if(ay != old_ay)
      {
         PutCoord(OAX, JAY_COORD, - textwidth("Y = "), ay);
         old_ay = ay;
      }


         /* Apply the gain divisor to the new coordinates and  */
         /* update the cross hair.                             */

      ax /= GAIN; ay /= GAIN;
      CrossHair(OAX + ax, OAY - ay);


         /* If we are in Gport mode, call GamAxis or GamStick to */
         /* get the joystick position for joystick B             */

      if(gport)
      {
#if AXIS
         bx = GamAxis(JBX);
         by = GamAxis(JBY);
#else
         GamAxes(JB, &bx, &by);
#endif
      }


         /* If either coordinate has changed since the last time  */
         /* it was checked, the screen must be updated with the   */
         /* new value                                             */

      if(bx != old_bx)
      {
         PutCoord(OBX, JBY_COORD, textwidth("  +0000") , bx);
         old_bx = bx;
      }
      if(by != old_by)
      {
         PutCoord(OBX, JBY_COORD, - textwidth("Y = "), by);
         old_by = by;
      }


         /* Apply the gain divisor to the new coordinates and  */
         /* update the cross hair.                             */

      bx /= 7; by /= GAIN;
      CrossHair(OBX + bx, OBY - by);


         /* If we are in Gport mode, use GamButton. Otherwise */
         /* make the call to the bios.                        */

      if(gport)
         bstat = GamButton();
      else
      {
         regs.h.ah = 0x84;
         regs.x.dx = 0;
         int86(0x15, &regs, &regs);
         bstat = regs.h.al ^ 0xf0;
      }


         /* Check the button status has changed from the    */
         /* last call. If it has, update the screen.        */

      if(bstat != old_bstat)
      {
         Button(bstat);
         old_bstat = bstat;
      }


         /* If the spacebar has been pressed, toggle modes. The   */
         /* screen is updated to reflect the new mode and a call  */
         /* to GamButton is made to clear the button status.      */

      if(bioskey(1))
      {
         key = bioskey(0);
         switch(key)
         {
            case F1:
               GamCalAxis(JAX, GamRawAxis(JAX));
               break;

            case F2:
               GamCalAxis(JAY, GamRawAxis(JAY));
               break;

            case F3:
               GamCalAxis(JBX, GamRawAxis(JBX));
               break;

            case F4:
               GamCalAxis(JBY, GamRawAxis(JBY));
               break;

            case F5:
               if(gport)
               {
                  gerase(BASE_X/2, TEXT_Y + 20, bkcolor, J_CENTER, *(cmode + center));
                  center ^= 1;
                  GamCenter(center);
                  gputs(BASE_X/2, TEXT_Y + 20, tcolor, J_CENTER, *(cmode + center));
                  break;
               }
               break;

            case F6:
               gerase(BASE_X/2, TEXT_Y, bkcolor, J_CENTER, *(mode + gport));
               gport ^= 1;
               gputs(BASE_X/2, TEXT_Y, tcolor, J_CENTER, *(mode + gport));
               if(!gport)
                  gerase(BASE_X/2, TEXT_Y + 20, bkcolor, J_CENTER, *(cmode + center));
               else
                  gputs(BASE_X/2, TEXT_Y + 20, tcolor, J_CENTER, *(cmode + center));

               Button(bstat);
               break;

            case F7:
               GamOpen();

               regs.h.ah = 0x84;
               regs.x.dx = 1;
               int86(0x15, &regs, &regs);
               bios_ax0 = regs.x.ax;
               bios_ay0 = regs.x.bx;
               bios_bx0 = regs.x.cx;
               bios_by0 = regs.x.dx;

               gerase(BASE_X/2, TEXT_Y + 20, bkcolor, J_CENTER, *(cmode + center));
               break;

         }
      }
   }

   Uninit();

}


/*
//-------------------------------------
//
// initialize
//
//-------------------------------------
*/

void Init(void)
{
int   mode;                   /* display mode         */
int   i;                      /* iterations           */
int   driver = DETECT;        /* display driver       */
union REGS regs;              /* registers for bios   */


      /* Initialize the game port for Gport and the bios  */

   if(!GamOpen())
   {
      puts("\nGame port not detected");
      exit(1);
   }


   regs.h.ah = 0x84;
   regs.x.dx = 1;
   int86(0x15, &regs, &regs);
   bios_ax0 = regs.x.ax;
   bios_ay0 = regs.x.bx;
   bios_bx0 = regs.x.cx;
   bios_by0 = regs.x.dx;


      /* Detect and initialize the graphics display */

   detectgraph(&driver,&mode);
   switch(driver)
   {
      case CGA:
         mode = CGAC0;
         color = CGA_YELLOW;
         bcolor = CGA_RED;
         vcolor = CGA_YELLOW;
         tcolor = CGA_GREEN;
         hcolor = CGA_RED;
         bkcolor = DARKGRAY;
         registerbgidriver(CGA_driver);
         break;

      case EGAMONO:
         color = LIGHTGRAY;
         bcolor = LIGHTGRAY;
         tcolor = LIGHTGRAY;
         vcolor = BLACK;
         hcolor = WHITE;
         registerbgidriver(EGAVGA_driver);
         break;

      case HERCMONO:
         bcolor = WHITE;
         vcolor = BLACK;
         hcolor = WHITE;
         bkcolor = BLACK;
         registerbgidriver(Herc_driver);
         break;

      default:
         registerbgidriver(EGAVGA_driver);
         break;
   }
   registerbgifont(small_font);
   initgraph(&driver,&mode,0);

   Xasp = BASE_X * XREF / (getmaxx() + 1);
   Yasp = BASE_Y * YREF / (getmaxy() + 1);
   cleardevice();
   setbkcolor(bkcolor);
   settextstyle(SMALL_FONT, HORIZ_DIR, 4);


      /* Draw our stuff on the screen  */

   gputs(OAX, 15, tcolor, J_CENTER, "Joystick A");
   gputs(OBX, 15, tcolor, J_CENTER, "Joystick B");
   setcolor(color);
   PutAxis(OAX, OAY);
   PutAxis(OBX, OBY);
   for(i=-BSPACE/2; i<BSPACE; i+=BSPACE)
   {
      PutButton(OAX + i, BAY);
      PutButton(OBX + i, BBY);
   }

   gerase(BASE_X/2, TEXT_Y, bkcolor, J_CENTER, "Gport");
   gputs(BASE_X/2, TEXT_Y, tcolor, J_CENTER, "Gport");
   gputs(BASE_X/2, TEXT_Y + 70, tcolor, J_CENTER, "Press ESC to quit");
   setcolor(color);


}


/*
//-------------------------------------
//
// un-initialize
//
//-------------------------------------
*/

void Uninit(void)
{
   GamClose();
   closegraph();
   free(old_image[0]);
   free(old_image[1]);
   exit(0);
}


/*
//-------------------------------------
//
// Draw the axes for a joystick
//
//-------------------------------------
*/

void PutAxis(int x, int y)
{
int i;
static j = 0;


      /* Draw the horizontal axis  */

   line(X(x - JR), Y(y), X(x + JR), Y(y));
   for(i=TSPACE; i<TNUM*(TSPACE*2+1); i+=TSPACE)
   {
      moveto(X(x - TSPACE*(TNUM+1) + i), Y(y - TSIZE));
      linerel(0, Y(TSIZE * 2));
   }


      /* Draw the vertical axis  */

   line(X(x), Y(y - JR), X(x), Y(y + JR));
   for(i=TSPACE; i<TNUM*(TSPACE*2 + 1); i+=TSPACE)
   {
      moveto(X(x - TSIZE), Y(y - TSPACE*(TNUM+1) + i));
      linerel(X(TSIZE * 2), 0);
   }

   gputs(x, y + Y_COORD_OFF, color, J_CENTER, "X =        Y =        ");


      /*    Allocate memory for storing the image behind the cross   */
      /* hair, and store the initial image behind the initial cross  */
      /* hair. Because this memory is only allocated once and the    */
      /* same area on different parts of the screen may need a       */
      /* different amount of memory, twice the approximate memory    */
      /* needed is allocated just to be safe.                        */

   old_image[j] = malloc(imagesize(0, 0, X(HSIZE * 2), Y(HSIZE * 2)) * 2);
   getimage(X(x - HSIZE), Y(y - HSIZE), X(x + HSIZE + 1), Y(y + HSIZE), old_image[j]);
   CrossHair(x, y);
   j++;
}


/*
//-------------------------------------
//
// put cross hair at x,y
//
//-------------------------------------
*/

void CrossHair(int x, int y)
{
static i = 1;
static int old_x[2] = { OAX , OBX };   /* old crosshair for joystick B  */
static int old_y[2] = { OAY , OBY };   /* old crosshair for joystick A  */
int old_color;                         /* current color storage         */


      /* Restore the old image behind the cross hair, saving the     */
      /* new coordinates for the next call. By XORing 'i' with 1, we */
      /* flip flop between the 2 joystick                            */

   i ^= 1;

   if(x == *(old_x + i) && y == *(old_y + i))
      return;


      /* If the coordinate has not changed there is no need to    */
      /* update the screen.                                       */

   putimage(X(*(old_x + i) - HSIZE), Y(*(old_y + i) - HSIZE), *(old_image + i), 0);
   getimage(X(x - HSIZE), Y(y - HSIZE), X(x + HSIZE + 1), Y(y + HSIZE), *(old_image + i));


      /* Draw the new cross hair */

   old_color = getcolor();
   setcolor(hcolor);
   moveto(X(x), Y(y - HSIZE));
   linerel(0, Y(HSIZE * 2));
   moverel(X(-HSIZE), Y(-HSIZE));
   linerel(X(HSIZE * 2), 0);
   setcolor(old_color);

   *(old_x + i) = x;
   *(old_y + i) = y;
}


/*
//-------------------------------------
//
// display a button
//
//-------------------------------------
*/

void PutButton(int x, int y)
{
   circle(X(x), Y(y), X(BR));
   circle(X(x), Y(y), X(BR - 3));
}


/*
//-------------------------------------
//
// display new button status
//
//-------------------------------------
*/

void Button(byte status)
{
int i = 0;
byte _button;
static byte button[4] = { 0, 0, 0, 0 };
int y[4] = { BAY, BAY, BBY, BBY };
int x[4] = { OAX - BSPACE / 2, OAX + BSPACE / 2,
             OBX - BSPACE / 2, OBX + BSPACE / 2
           };

      /* Or the pending status with the current status to determine  */
      /* if a button has been pressed                                */

   status |= (status >> 4);

      /* Check each button for a change in it's state by shifting       */
      /* the bits right once each iteration and anding with 1, and      */
      /* then comparing it's new status with it's old status. If it's   */
      /* status has changed, redraw the button accordingly              */

   while(i < 4)
   {
      _button = status & 1;
      if(button[i] != _button)
      {
         button[i] = _button;
         setfillstyle(_button, bcolor);
         fillellipse(X(x[i]), Y(y[i]), X(BR - 3), Y(BR - 3));
      }
      status >>= 1;
      i++;
   }
}



/*
//-------------------------------------
//
// put a coordinate value
//
//-------------------------------------
*/

void PutCoord(int x, int y, int just, signed value)
{
char     str[80];

   sprintf(str, "%+04d", value);
   gerase(x, y, bcolor, just, str);
   gputs(x, y, vcolor, just, str);

}


/*
//-------------------------------------
//
// graphics puts
//
//-------------------------------------
*/

void gputs(int x, int y, int color, int just, char *str)
{

   switch(just)
   {
      case J_CENTER:
         just = textwidth(str) / 2;

      case J_LEFT:
         break;

      case J_RIGHT:
         just = textwidth(str);

      default:
         break;
   }

   setcolor(color);
   outtextxy(X(x) - just, Y(y), str);
   setcolor(color);
}


/*
//-------------------------------------
//
// graphics erase
//
//-------------------------------------
*/

void gerase(int x, int y, int color, int just, char *str)
{
struct   fillsettingstype old_set;


   switch(just)
   {
      case J_CENTER:
         just = textwidth(str) / 2;

      case J_LEFT:
         break;

      case J_RIGHT:
         just = textwidth(str);
   }

   getfillsettings(&old_set);
   setfillstyle(SOLID_FILL, color);
   bar(X(x) - just - 2, Y(y), X(x) + textwidth(str) - just + 1, Y(y) + textheight(str) + 3);
   setfillstyle(old_set.pattern, old_set.color);

}
