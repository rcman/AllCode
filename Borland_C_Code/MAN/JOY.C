#include <stdio.h>
#include <dos.h>
#include <bios.h>

typedef struct {
    int sw1,sw2;
    int x,y;
    int cenx,ceny;
    } joy_stick;

joy_stick joy;

#define keypressed (bioskey(1) != 0)
#define kb_clear() while keypressed bioskey(0);

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
    GotoXY(0,line);
    printf("sw1 %d sw2 %d",joy -> sw1,joy -> sw2);
    GotoXY(0,line+1);
    printf("x %4d y %4d",joy -> x,joy -> y);
}

void read_stick(int stick,joy_stick *joy)
{
    int k,jx,jy;
    int c,m1,m2,m3,m4,m5;

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

