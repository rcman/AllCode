/* CONFIG.C is the main screen and config routines */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "tankdefs.h"
#include "types.h"
#include "extern.h"
#include "joy.h"

extern joy_stick joystick;
extern int JoyStickNumber;
extern int StickOnTank;

#define UPKEY    0x4800
#define DOWNKEY  0x5000
#define ESCKEY   0x011b
#define SPACEKEY 0x3920
#define RETKEY   0x1c0d

#define YBASE 11
#define X 21

/* define menu order */
#define BEGIN		0
#define SCORES		1
#define GUIDED		2
#define TREES		3
#define TDELAY		4
#define SDELAY		5
#define SRANGE		6
#define NTANKS		7
#define JOYTANK		8
#define CALJOY		9
#define SCONFIG		10
#define DEFAULTS	11

void drawconfig()
{
	clrscr();
	textattr(0x0e);
	gotoxy(18,1); cprintf("ษอออออออออออออออออออออออออออออออออออออออป");
	gotoxy(18,2); cprintf("บ            SUPER TANK 1.1             บ");
	gotoxy(18,3); cprintf("บ                                       บ");
	gotoxy(18,4); cprintf("บ  (C) Copyright 1992 Kevin Dahlhausen  บ");
   gotoxy(18,5); cprintf("บ                                       บ");
   gotoxy(18,6); cprintf("ศอออออออออออออออออออออออออออออออออออออออผ");
   textattr(0x07);
}

void updatescreen()
/* requires: none
 * ensures:  current values of config variables are written,
 * 			 starting at YBASE,X
 */
{  int x=wherex(), y=wherey();
	gotoxy(X,YBASE+BEGIN); 		textattr(0x07); cprintf("Begin game");

	gotoxy(X,YBASE+SCORES);		textattr(0x07); cprintf("Reset scores");

	gotoxy(X,YBASE+GUIDED); 	textattr(0x0f);
	if (GuidedShots) cprintf(" ON"); else cprintf("OFF");
	textattr(0x07); cprintf(" Guided Bullets");

	gotoxy(X,YBASE+TREES); 	textattr(0x0f);
	if (TreesOn) cprintf(" ON"); else cprintf("OFF");
	textattr(0x07); cprintf(" Trees Option");

	gotoxy(X,YBASE+TDELAY);
	textattr(0x0f); cprintf("%03d", TankSpeed);
	textattr(0x07); cprintf(" Tank Delay");

	gotoxy(X,YBASE+SDELAY);
	textattr(0x0f); cprintf("%03d", ShotSpeed);
	textattr(0x07); cprintf(" Bullet Delay");

	gotoxy(X,YBASE+SRANGE);
	textattr(0x0f); cprintf("%03d",ShotRange);
	textattr(0x07); cprintf(" Bullet Range");

	gotoxy(X,YBASE+NTANKS);
	textattr(0x0f); cprintf("%03d", NumTanks);
	textattr(0x07); cprintf(" Number of players");

	gotoxy(X,YBASE+JOYTANK);
	textattr(0x0f);
	if (StickOnTank==-1) cprintf("OFF");
	else cprintf("%03d", StickOnTank+1);
	textattr(0x07); cprintf(" Joystick on tank");


	gotoxy(X,YBASE+CALJOY);
	textattr(0x07); cprintf("Calibrate joystick");

	gotoxy(X, YBASE+1+SCONFIG); cprintf("Save configuration");
	gotoxy(X, YBASE+1+DEFAULTS); cprintf("Restore default values");

	gotoxy(x,y); /* restore original cursor position */
}

void saveconfig()
/* requires: none
 * ensures: config is saved to CONFIGFILE
 */
{ FILE *cfile;
  if (cfile=fopen(CONFIGFILE, "wt")) {
	 fprintf(cfile, "trees=%d\n", TreesOn);
	 fprintf(cfile, "guided=%d\n", GuidedShots);
	 fprintf(cfile, "tankspeed=%d\n", TankSpeed);
	 fprintf(cfile, "shotspeed=%d\n", ShotSpeed);
	 fprintf(cfile, "shotrange=%d\n", ShotRange);
	 fprintf(cfile, "numtanks=%d\n",  NumTanks);
	 fprintf(cfile, "stickon=%d\n", StickOnTank);
	 fprintf(cfile, "sticknumber=%d\n", JoyStickNumber);
	 fprintf(cfile, "cenx=%d\n", joystick.cenx);
	 fprintf(cfile, "ceny=%d\n", joystick.ceny);
	 fprintf(cfile, "tu=%d\n", joystick.tu);
	 fprintf(cfile, "td=%d\n", joystick.td);
	 fprintf(cfile, "tl=%d\n", joystick.tl);
	 fprintf(cfile, "tr=%d\n", joystick.tr);
	 fclose(cfile);
 }
}


void CalibrateStick()
/* requires: none
 * ensures:  joystick initialized
 */
{
	clrscr();
	drawconfig();
	printf("\n\n\n\n");
	/* joystick routines */
	JoyStickNumber=choose_stick();
	set_thresh(JoyStickNumber, &joystick);
	clrscr();
	drawconfig();
}

int configmenu()
/* handles the config options in opening screen */
{
 int state=0;
 int keyp, i;
 int cpos;
 int returnstate;
 int *valptr;

 CursorOff;
 drawconfig();
 while(1) {
	updatescreen();
	switch (state) {
		case BEGIN:  gotoxy(X-3,YBASE+BEGIN); printf("->");
					switch(bioskey(0)) {
					case RETKEY:
					case SPACEKEY:	if ((StickOnTank>-1)&&(JoyStickNumber==-1)) {
							 			CalibrateStick();
									}
									return (1);  /* begin game */
					case ESCKEY:	return (0);  /* decided not to play */
               		case UPKEY:    state=DEFAULTS; break;
					case DOWNKEY:	state++; break;
				  };
				  gotoxy(X-3, YBASE+BEGIN); printf("  ");
				  break;

		case SCORES:  gotoxy(X-3,YBASE+SCORES); printf("->");
				switch(bioskey(0)) {
					case ESCKEY:	return (0);  /* decided not to play */
					case UPKEY:		state--; break;
					case DOWNKEY:	state++; break;
					case RETKEY:
					case SPACEKEY: for(i=0; i<MAXNUMTANKS; i++) tank[i].score=0;
				 };
				 gotoxy(X-3, YBASE+SCORES); printf("  ");
				 break;


		case GUIDED:  gotoxy(X-3,YBASE+GUIDED); printf("->");
					switch(bioskey(0)) {
					case ESCKEY:	return (0);  /* decided not to play */
					case UPKEY:		state--; break;
					case DOWNKEY:	state++; break;
					case RETKEY:
					case SPACEKEY: GuidedShots = !GuidedShots;
				  };
				  gotoxy(X-3, YBASE+GUIDED); printf("  ");
				  break;


		case TREES:  gotoxy(X-3,YBASE+TREES); printf("->");
					switch(bioskey(0)) {
					case ESCKEY:	return (0);  /* decided not to play */
					case UPKEY:		state--; break;
					case DOWNKEY:	state++; break;
					case RETKEY:
					case SPACEKEY: TreesOn = !TreesOn;
				  };
				  gotoxy(X-3, YBASE+TREES); printf("  ");
				  break;

		case TDELAY: gotoxy(X-3,YBASE+TDELAY); printf("->");
				  switch(keyp=bioskey(0)) {
					 case ESCKEY:	 return (0);  /* decided not to play */
					 case UPKEY:	 state--; break;
					 case DOWNKEY:	 state++; break;
					 default:       keyp=(keyp&0x00ff)-0x30;  /* check for # key */
										 if ((keyp>=0)&&(keyp<=9)) {
											returnstate=TDELAY;
											valptr=&TankSpeed;
											state=101;
											gotoxy(X, YBASE+TDELAY);
											CursorOn;
										 }
				  };
				  gotoxy(X-3, YBASE+TDELAY); printf("  ");
				  break;

		case SDELAY: gotoxy(X-3,YBASE+SDELAY); printf("->");
				  switch(keyp=bioskey(0)) {
					 case ESCKEY:	 return (0);  /* decided not to play */
					 case UPKEY:	 state--; break;
					 case DOWNKEY:	 state++; break;
					 default:       keyp=(keyp&0x00ff)-0x30;  /* check for # key */
										 if ((keyp>=0)&&(keyp<=9)) {
											returnstate=SDELAY;
											valptr=&ShotSpeed;
											state=101;
											gotoxy(X, YBASE+SDELAY);
											CursorOn;
										 }
				  };
				  gotoxy(X-3, YBASE+SDELAY); printf("  ");
				  break;

		case SRANGE: gotoxy(X-3,YBASE+SRANGE); printf("->");
				  switch(keyp=bioskey(0)) {
					 case ESCKEY:	 return (0);  /* decided not to play */
					 case UPKEY:	 state--; break;
					 case DOWNKEY:	 state++; break;
					 default:       keyp=(keyp&0x00ff)-0x30;  /* check for # key */
										 if ((keyp>=0)&&(keyp<=9)) {
											returnstate=SRANGE;
											valptr=&ShotRange;
											state=101;
											gotoxy(X, YBASE+SRANGE);
											CursorOn;
										 }
				  };
				  gotoxy(X-3, YBASE+SRANGE); printf("  ");
				  break;

		case NTANKS:	gotoxy(X-3, YBASE+NTANKS); printf("->");
					switch(keyp=bioskey(0)) {
						case ESCKEY:	return(0);
						case UPKEY:		state--; break;
						case DOWNKEY:	state++; break;
						case 0x0332:			NumTanks=2;
											for (i=0; i<NumTanks; i++) tank[i].score=0;
											break;
						case 0x0433:			NumTanks=3;
											for (i=0; i<NumTanks; i++) tank[i].score=0;
											break;
						case SPACEKEY:
						case RETKEY:	NumTanks=((NumTanks-1)%(MAXNUMTANKS-1))+2;
											for (i=0; i<NumTanks; i++) tank[i].score=0;

				  }

				  gotoxy(X-3, YBASE+NTANKS); printf("  ");
				  break;

		case JOYTANK: gotoxy(X-3, YBASE+JOYTANK); printf("->");
					switch(bioskey(0)) {
						case ESCKEY:	return(0);
						case UPKEY:		state--; break;
						case DOWNKEY:	state++; break;
						case RETKEY:
						case SPACEKEY:   StickOnTank++;
										 if (StickOnTank==MAXNUMTANKS) StickOnTank=-1;
						 				 break;
					}
					gotoxy(X-3, YBASE+JOYTANK); printf("  ");
					break;

		case CALJOY: gotoxy(X-3, YBASE+CALJOY); printf("->");
					switch(bioskey(0)) {
						case ESCKEY:	return(0);
						case UPKEY:		state--; break;
						case DOWNKEY:	state++; break;
						case RETKEY:
						case SPACEKEY:  CalibrateStick();
										break;

					}
					gotoxy(X-3, YBASE+CALJOY); printf("  ");
					break;


		case SCONFIG:  gotoxy(X-3, YBASE+SCONFIG+1); printf("->");
					switch(bioskey(0)) {
						case ESCKEY:	return(0);
						case UPKEY:		state--; break;
						case DOWNKEY:	state++; break;
						case RETKEY:
						case SPACEKEY:   saveconfig(); break;
					}
					gotoxy(X-3, YBASE+SCONFIG+1); printf("  ");
					break;

		case DEFAULTS:	gotoxy(X-3,YBASE+DEFAULTS+1); printf("->");
					switch(bioskey(0)) {
						case ESCKEY:	return(0);
						case UPKEY:		state--; break;
                  case DOWNKEY:  state=0; break;
						case RETKEY:
						case SPACEKEY:	GuidedShots=0;
											TreesOn=0;
											TankSpeed=DEFTANKSPEED;
											ShotSpeed=DEFSHOTSPEED;
											ShotRange=DEFSHOTRANGE;
											NumTanks=DEFNUMTANKS;
											break;
					}
					gotoxy(X-3, YBASE+DEFAULTS+1); printf("  ");
					break;

		case 101: /* keyp contains 0-9 */
				  		*valptr=keyp*100;
						state=102;
						gotoxy(wherex()+2, wherey());
						break;

		case 102: /* 2nd digit */
					   keyp=(bioskey(0)&0x00ff)-0x30;
					   if ((keyp>=0)&&(keyp<=9)) {
						   *valptr += (keyp*10);
						   state=103;
						   gotoxy(wherex()+1, wherey());
					   }
					   break;

		case 103: /* 3rd digit */
					 keyp=(bioskey(0)&0x00ff)-0x30;
					 if ((keyp>=0)&&(keyp<=9)) {
						 *valptr += keyp;
						 state=returnstate;
						 CursorOff;
					 }
					 break;

	} /* main switch */
 } /* while(1) */
}

void readconfig()
/* requires: none
 * ensures:  config is read from data file
 */
{
	FILE *cfile;
	char nextline[81];
	char string[81];
	int  newval;
	int eqpos;

 /* parse config file */
 if (cfile=fopen(CONFIGFILE,"rt")) {
	while (fgets(nextline, 80, cfile)!=NULL) {
		if (eqpos=strcspn(nextline,"=")) {
			strncpy(string, nextline, eqpos);  /* contains the arguement */
			string[eqpos]='\0';
			strlwr(string);
			newval=atoi(nextline+eqpos+1);
			if (strcmp(string,"trees")==0) 	       TreesOn=newval;
			else if (strcmp(string,"guided")==0)	 GuidedShots=newval;
			else if (strcmp(string,"shotrange")==0) ShotRange=newval;
			else if (strcmp(string,"shotspeed")==0) ShotSpeed=newval;
			else if (strcmp(string,"tankspeed")==0) TankSpeed=newval;
			else if (strcmp(string,"numtanks")==0)  NumTanks=newval;
			else if (strcmp(string,"cenx")==0) joystick.cenx=newval;
			else if (strcmp(string,"ceny")==0) joystick.ceny=newval;
			else if (strcmp(string,"tu")==0) joystick.tu=newval;
			else if (strcmp(string,"td")==0) joystick.td=newval;
			else if (strcmp(string,"tl")==0) joystick.tl=newval;
			else if (strcmp(string,"tr")==0) joystick.tr=newval;
			else if (strcmp(string,"stickon")==0) StickOnTank=newval;
			else if (strcmp(string,"sticknumber")==0) JoyStickNumber=newval;
		}
	}
	fclose(cfile);
	}
 }
