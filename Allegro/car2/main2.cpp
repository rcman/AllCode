
#include <allegro.h>

int waitbutton = 0;
int shotsalive = 1;
int mytic = 0;
int tempy;
BITMAP* BackBuffer;
BITMAP* Bullet;
BITMAP* Ship;
BITMAP* Shot;

int bAppDone = 0;
volatile int ticketyticktick;
void tickety ()
{
 ticketyticktick++;
}
END_OF_FUNCTION()

///////////////////////////////////////////////////////////////////////////////
typedef struct Vector2D { int x, y; } Vector2D;
typedef struct Bounds { Vector2D TopLeft, BottomRight; } Bounds;
typedef struct PhysicsModel { Vector2D position, velocity; Bounds Box; };
typedef struct Shots
       {
       int x,y,x1,y1,start;           // position of ship
       } SHOTS;
       
SHOTS allshots[1];

int ShipX=320;
int ShipY=620;
int shotcount = 0;
int totalshots = 1;
int i;  // for loop



class CBullet
{
private:
 PhysicsModel m_kModel;
public:
 CBullet ();
 ~CBullet();
 void Create (){};
 void Update (){};
 void Render (){};
 void Destroy()         { if (this!=NULL){delete this;} };
 void set(int params[])             
 { 
 };
 void set(int x, int y, int vx, int vy, int l, int t, int r, int b)
 {
 };
 void set(Vector2D pos, Vector2D vel)
 {
 };
 int x()                { return m_kModel.position.x; };
 int y()                { return m_kModel.position.y; };
 int vx()               { return m_kModel.velocity.x; };
 int vy()               { return m_kModel.velocity.y; };
 int left()             { return m_kModel.Box.TopLeft.x; };
 int top()              { return m_kModel.Box.TopLeft.y; };
 int right()            { return m_kModel.Box.BottomRight.x; };
 int bottom()           { return m_kModel.Box.BottomRight.y; };
 Bounds bounds()        { return m_kModel.Box; };
 Vector2D position()    { return m_kModel.position; };
 Vector2D velocity()    { return m_kModel.velocity; };
};


void initialize_game ()
{
 Ship = load_bitmap("red_ship3.bmp", NULL);
 if (Ship==NULL)
 { 
  alert("Loading red_ship3.bmp","Failed",":(","OK",NULL,KEY_ENTER,KEY_ESC);
  bAppDone=1;
 }
 Shot = load_bitmap("shot_single.bmp", NULL);
 if (Shot==NULL)
 { 
  alert("Loading shot_single.bmp","Failed",":(","OK",NULL,KEY_ENTER,KEY_ESC);
  bAppDone=1;
 }
 
 
 
}

void shutdown_game ()
{
 if (BackBuffer!=NULL) { destroy_bitmap (BackBuffer); }
 if (Bullet!=NULL)     { destroy_bitmap (Bullet); }
 if (Ship!=NULL)       { destroy_bitmap (Ship); }
}

void update_logic ()
{
 if (key[KEY_ESC])
 {
  bAppDone=1;
 }
 if (key[KEY_UP])
 {
   // y = y -1
   ShipY = ShipY - 3;
   if (ShipY < 1)
      {
            ShipY = 1;
  }    
 }
 if (key[KEY_DOWN])
 {
   // y = y + 1
   ShipY = ShipY + 3;
   if (ShipY > 768 - Ship->h)
      {
            ShipY = 768 - Ship->h;
  }    
 }
 if (key[KEY_LEFT])
 {
   // x = x - 1 
   ShipX = ShipX - 3;
      if (ShipX < 1)
            {
            ShipX = 1;
        }    
 }
 if (key[KEY_RIGHT])
 {
   // x = x + 1
   ShipX = ShipX + 3;
      if (ShipX > 1023 - Ship->w)
            {
                ShipX = 1023 - Ship->w;
        }    
 }
 if (key[KEY_LCONTROL])
 {
   if (shotsalive >= 999)
   {
    shotsalive = 1;
   }
    
   allshots[shotsalive].x = ShipX-6;
   allshots[shotsalive].y = ShipY-10;
   allshots[shotsalive].x1 = ShipX+56;
   allshots[shotsalive].y1 = ShipY-10;
   allshots[shotsalive].start = ShipY;
   
   shotsalive = shotsalive + 1;
                             
  }

} 

void MoveShots()
{    
    for (i = 1;i <= shotsalive; i++)
    {
       if (allshots[i].y > 2)
       {      
       allshots[i].y = allshots[i].y - 4;
       allshots[i].y1 = allshots[i].y1 - 4;
       draw_sprite (BackBuffer, Shot, allshots[i].x, allshots[i].y); //test
       draw_sprite (BackBuffer, Shot, allshots[i].x1, allshots[i].y1); //test
     }    
      if (allshots[i].y < 1)
              {
              allshots[i].y =1;
              allshots[i].y1 =1;
              }
              tempy = allshots[i].y1 - allshots[i].y;
                   if (tempy > 90)
                   {
                   waitbutton = 0;
               }
      }                  
          
}
  
 
 //  if (key[KEY_UP]) { ... } 
  //  if (key[KEY_DOWN]) { ... } 
  //  if (key[KEY_LEFT]) { ... } 
  //  if (key[KEY_RIGHT]) { ... } 
  //  if (key[KEY_SPACE]) { ... } 
  //  key[KEY_LCONTROL] 
   // key[KEY_RCONTROL] 
  //  key[KEY_LSHIFT] 
  //  key[KEY_RSHIFT] 


 

///////////////////////////////////////////////////////////////////////////////

void update_graphics ()
{ 
    clear_bitmap (BackBuffer); 
 draw_sprite (BackBuffer, Ship, ShipX, ShipY);
 MoveShots(); //draw_sprite (BackBuffer, Shot, 200, 200); //test
  
 blit (BackBuffer, screen, 0, 0, 0, 0, 1024, 768);
 
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
 set_gfx_mode (GFX_AUTODETECT_WINDOWED, 1024, 768, 0, 0);
 BackBuffer = create_bitmap (1024, 768); clear_bitmap (BackBuffer);
 
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

