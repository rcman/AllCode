/* EXTERN.H
 * This file contains all external declarations for the tank program.
 * It is to be included in all modules. The variables are defined in
 * TANK.C
 */

/* variables that reflect the current configuration */
extern int NumTanks;				/* number tanks in current game */
extern int TankSpeed;			/* number of main loops between tank movements */
extern int ShotSpeed;			/* number of main loops between shot movements */
extern int ShotRange;
extern int TreesOn;				/* non-zero if Trees option is on */
extern int GuidedShots;			/* non-zero if tank shots are guided */

extern char far *vbuf;       	/* pointer to video buffer */
extern int  tankColor[];     	/* color of each tank */
extern int  TankInitialX[];   /* X coordinate of tank at start of each round */
extern int	TankInitialY[];   /* Y corrdinate of tank at start of each round */
extern tankRec tank[];       	/* data for each tank */
extern shotrec shotdata[];   	/* data for each shot */
extern treeRec tree[];       	/* coordinates of each tree */
extern int	newscreen;			/* non-zero means start a new game    */




extern unsigned char field[FIELDX][FIELDY];
/* Field contains the id number of the object at that x,y position.
 *	It is used to determine whic of multiple objects of the same
 *	color (like trees) occupy that position. Any objects that use this
 *	must set pixels corresponding to each non-zero element in its bitmap.
 *	Due to memory considerations, it is valid only for X<FIELDX, Y<FIELDY.
 *	This is not a problem, as there should always be one row free of objects
 *	around the screen border.
 */

extern int HitX; /* X coordinate that caused a collision, set by HIT() */
extern int HitY; /* Y coordinate that caused a collision, set by HIT() */

/* picture definitions (bitmaps) */
extern char TankUpBM[];
extern char TankDownBM[];
extern char TankLeftBM[];
extern char TankRightBM[];
extern char TankEraseBM[];
extern char ShotBM[];
extern char ShotEraseBM[];
extern char	Explosion1BM[];
extern char Explosion1EraseBM[];
extern char TreeBM[];
extern char TreeEraseBM[];


/* functions */
extern int  hit();       		/* detects collisions, sets HitX,HitY */
extern void drawit();    		/* places bit-maps on screen */
extern void restart();			/* sets graphic mode, resets game */
extern void shots();				/* handles creation/movement of tank shots */
extern void TankHit();			/* called when tank gets hit */
extern void mode();				/* sets video mode, optionally clears screen */
extern void DisplayChar13();	/* put character on screen in graphics mode */
extern void PutString13();		/* put string on screen in graphics mode */
extern int config();
extern void SetCursor();

/* Keyboard routine */
extern char gr_keys[128];
void gr_start_kbd_grab(void);
void gr_end_kbd_grab(void);
/* MUST BE COMPILED WITHOUT STACK CHECKS AND REGISTER VARIABLES */
/** Vector to the old keyboard interrupt handler **/
extern void interrupt (*gr_old_int9)(void);
void interrupt gr_int9(void);