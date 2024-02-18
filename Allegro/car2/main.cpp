
#include <allegro.h>

BITMAP* BackBuffer;
BITMAP* car1;
BITMAP* car2;
BITMAP* car3;
int t;
int x=350;
int y=430;
int speed=1;
int yvel=2;
int xvel=2;
int jump=0;
int bAppDone = 0;
volatile int ticketyticktick;
void tickety ()
{
 ticketyticktick++;
}
END_OF_FUNCTION()

void initialize_game ()
{
 car1 = load_bitmap("car3_1.bmp", NULL);
 if (car1==NULL)
 { 
  alert("Loading car3_1.bmp","Failed",":(","OK",NULL,KEY_ENTER,KEY_ESC);
  bAppDone=1;
 }
 car2 = load_bitmap("car3_2.bmp", NULL);
 if (car2==NULL)
 { 
  alert("Loading car3_2.bmp","Failed",":(","OK",NULL,KEY_ENTER,KEY_ESC);
  bAppDone=1;
 }
 car3 = load_bitmap("car3_0.bmp", NULL);
 if (car3==NULL)
 { 
  alert("Loading car3_3.bmp","Failed",":(","OK",NULL,KEY_ENTER,KEY_ESC);
  bAppDone=1;
 }
 
}

void shutdown_game ()
{
 if (BackBuffer!=NULL) { destroy_bitmap (BackBuffer); }
 if (car1!=NULL)     { destroy_bitmap (car1); }
 if (car2!=NULL)       { destroy_bitmap (car2); }
 if (car3!=NULL)       { destroy_bitmap (car3); }
}

void update_logic ()
{
 if (key[KEY_ESC])
 {
  bAppDone=1;
 }
 if (key[KEY_UP] && jump ==0)
 {
   yvel = -60;
   jump = 1;
  } 
 
 if (jump == 1)
 {
     yvel = yvel + 8;
 } 
 if ( yvel > 10)
 { yvel =0;
   jump =0;
}
 x = x + xvel;  
   
 if (x > 550)
 {
     xvel = -2;
}
if (x < 10)
{
    xvel = 2;
}      

 
} 


//int y=200;
//int speed=2;
//int vely=2;


void update_graphics ()
{ 
 clear_bitmap (BackBuffer); 
 draw_sprite (BackBuffer, car1, x+xvel, y + yvel);
 //draw_sprite (BackBuffer, car2, 250, 200);
 // draw_sprite (BackBuffer, car3, 300, 200);
  blit (BackBuffer, screen, 0, 0, 0, 0, 640, 480);
 
}

///////////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY THE FOLLOWING CODE!
// ONLY ADD CODE ABOVE!
int main (int args, char* argv[])
{
 allegro_init ();
 install_timer ();
 install_keyboard ();
 set_color_depth  ( 16 );
 set_gfx_mode (GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
 BackBuffer = create_bitmap (640, 480); clear_bitmap (BackBuffer);
 
 LOCK_VARIABLE(ticketyticktick)
 LOCK_FUNCTION(tickety)
 install_int_ex(tickety, BPS_TO_TIMER(100));
 initialize_game ();
 while (bAppDone==0)
 {
  while (ticketyticktick > 0)
  {
   update_logic ();
   ticketyticktick--;
  }
  update_graphics();
 }
 shutdown_game ();
}
END_OF_MAIN()

