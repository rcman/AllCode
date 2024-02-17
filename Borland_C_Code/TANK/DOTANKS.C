/* DOTANKS:  Handle movement, collision of tanks */
#include "tankdefs.h"
#include "types.h"
#include "extern.h"
#include "stk.h"
#include "joy.h"

extern joy_stick joystick;
extern int JoyStickNumber;
extern int StickOnTank;

void dotanks()
{
	int i;
	int tx,ty;
	tankRec *t;
	shotrec *s;

	if (StickOnTank>-1) {
		read_stick(JoyStickNumber, &joystick);
		gr_keys[tank[StickOnTank].upk]=JoyStickUp(joystick);
		gr_keys[tank[StickOnTank].downk]=JoyStickDown(joystick);
		gr_keys[tank[StickOnTank].leftk]=JoyStickLeft(joystick);
		gr_keys[tank[StickOnTank].rightk]=JoyStickRight(joystick);
		gr_keys[tank[StickOnTank].firek]=JoyButton1(joystick)|JoyButton2(joystick);
	}

	for ( i=0; i<NumTanks; i++) {
		t=&tank[i];
		if (gr_keys[t->upk]) {
			if (t->dir==UP) {
				t->speed--;
				if (!t->speed) {
					t->speed=TankSpeed;
					tx=t->x; ty=t->y-1;
					if (!hit(tx,ty,TXDIM,TYDIM,UP)) {
						drawit(t->x,t->y,TXDIM,TYDIM,TankEraseBM);
						drawit(tx,ty,TXDIM,TYDIM,t->pic[UP]);
						t->y=ty;
					}
					else ;/* hit */
				}
			}
			else {
				drawit(t->x,t->y,TXDIM,TYDIM,TankEraseBM);
				drawit(t->x,t->y,TXDIM,TYDIM,t->pic[UP]);
				t->dir=UP;
				t->speed=TankSpeed;
			}
		}
		else if (gr_keys[t->downk]) {
			if (t->dir==DOWN) {
				t->speed--;
				if (!t->speed) {
					t->speed=TankSpeed;
					tx=t->x; ty=t->y+1;
					if (!hit(tx,ty,TXDIM,TYDIM,DOWN)) {
						drawit(t->x,t->y,TXDIM,TYDIM,TankEraseBM);
						drawit(tx,ty,TXDIM,TYDIM,t->pic[DOWN]);
						t->y=ty;
					}
					else ;/* hit */
				}
			}
			else {
				drawit(t->x,t->y,TXDIM,TYDIM,TankEraseBM);
				drawit(t->x,t->y,TXDIM,TYDIM,t->pic[DOWN]);
				t->dir=DOWN;
			}
		}
		else if (gr_keys[t->rightk]) {
			if (t->dir==RIGHT) {
				t->speed--;
				if (!t->speed) {
					t->speed=TankSpeed;
					tx=t->x+1; ty=t->y;
					if (!hit(tx,ty,TXDIM,TYDIM,RIGHT)) {
						drawit(t->x,t->y,TXDIM,TYDIM,TankEraseBM);
						drawit(tx,ty,TXDIM,TYDIM,t->pic[RIGHT]);
						t->x=tx;
					}
					else ;/* hit */
				}
			}
			else {
				drawit(t->x,t->y,TXDIM,TYDIM,TankEraseBM);
				drawit(t->x,t->y,TXDIM,TYDIM,t->pic[RIGHT]);
				t->dir=RIGHT;
			}
		}
		else if (gr_keys[t->leftk]) {
			if (t->dir==LEFT) {
				t->speed--;
				if (!t->speed) {
					t->speed=TankSpeed;
					tx=t->x-1; ty=t->y;
					if (!hit(tx,ty,TXDIM,TYDIM,LEFT)) {
						drawit(t->x,t->y,TXDIM,TYDIM,TankEraseBM);
						drawit(tx,ty,TXDIM,TYDIM,t->pic[LEFT]);
						t->x=tx;
					}
					else ;/* hit */
				}
			}
			else {
				drawit(t->x,t->y,TXDIM,TYDIM,TankEraseBM);
				drawit(t->x,t->y,TXDIM,TYDIM,t->pic[LEFT]);
				t->dir=LEFT;
			}
		}
  } /* for */
}