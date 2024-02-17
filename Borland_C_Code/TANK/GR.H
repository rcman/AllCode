/**********************************************************************
* gr.h
*
* Support routines for graphics mode.
**********************************************************************
                    This file is part of

          STK -- The sprite toolkit -- version 1.0

              Copyright (C) Jari Karjala 1990

The sprite toolkit (STK) is a FreeWare toolkit for creating high
resolution sprite graphics with PCompatible hardware. This toolkit 
is provided as is without any warranty or such thing. See the file
COPYING for further information.

**********************************************************************
**********************************************************************/

#if     !defined(__GR_H_)
#define __GR_H_


#define GR_TYPE_ANY 0   /* Any mode will do */
#define GR_TYPE_SPR 1   /* The best possible mode for the sprite toolkit */

/**********************************************************************
* Detect the graphics card and mode of the system.
* The type parameter can be used to specify special requests (see above).
*
* graphdriver and graphmode parameters are returned. These values can 
* be used with the gr_start function. They contain the value -1
* if some error occured (cannot find requested mode, etc)
**********************************************************************/
void gr_detect(int type, int *graphdriver, int *graphmode);

/***************************************************************************
* Initializes the graphics system. 
* Search BGI drivers from the path defined in the enviroment variable BGIDIR
* or current directory. 
* Set videomode into the BIOS to fool mouse drivers with Hercules graphics.
* Set gr_end at exit and ctrl-C signals.
* Terminate with error message if initialization fails.
*
* graphdriver   pointer to the driver ID (or DETECT)
* graphmode     pointer to the mode ID
***************************************************************************/
void gr_start(int *graphdriver, int *graphmode);

/***************************************************************************
* Returns to the text mode
***************************************************************************/
void gr_end(void);


/**********************************************************************
* gr_putch, gr_puts, gr_printf work as the as in text modes.
* '\n', '\r' are handled as in standard text modes. 
* Scrolling or backspacing not implemented.
**********************************************************************/
void gr_putch(char ch);
void gr_puts(char *s);
void gr_printf(char *s,...);
#define gr_gotoxy(x, y) moveto(x*8, y*8)

/**********************************************************************
* printf at the given position. (x and y in pixels) 
**********************************************************************/
void gr_xy_printf(int x, int y, char *s,...);

/**********************************************************************
* printf into both graphics pages at the given position.
* (x and y in pixels) Page 0 is left active.
**********************************************************************/
void gr_dual_xy_printf(int x, int y, char *s,...);

/**********************************************************************
* Return a keypress if one pending, otherwise 0.
* Extended codes contain 0 in the low byte.
*
* Automatic key repeat disabled by setting kbd buffer head and tail
* equal. 
**********************************************************************/
int gr_inkey(void);

/**********************************************************************
* Read a string from screen in graphics mode.
* The result is placed into cpdest, at most max_len characters are
* read (max_len must be less than 80). Return cpdest of NULL
* Backspace deletes characters, Esc returns a NULL pointer.
**********************************************************************/
char *gr_gets(char *cpdest, int max_len);

/**********************************************************************
* The maximum coordinate values for current graphics adapter.
* (It is faster to use variables instead of getmaxx() and getmaxy());
**********************************************************************/
extern int gr_max_x;
extern int gr_max_y;

/**********************************************************************
* This variable defines the text writing mode (default GR_MODE_CLEAR)
* (See the defines below)
**********************************************************************/
extern int gr_text_mode;

#define GR_MODE_OR      (1<<0)      /* OR the text over previous graphics */
#define GR_MODE_CLEAR   (1<<1)      /* Clear the backgroud before print  */


/**********************************************************************
* Array of booleans for each key of the keyboard (indexed by the scan
* code value). Non-zero if key pressed, zero otherwise.
* The array is updated during the kbd_grab, see the function 
* gr_start_kbd_grab below.
**********************************************************************/
extern char gr_keys[128];


/**********************************************************************
* Set a new handler for the keyboard. This handler sets and resets
* the gr_keys array values for each scancode received, then flushes the 
* keyboard buffer, and after that calls the old handler.
**********************************************************************/
void gr_start_kbd_grab(void);

/**********************************************************************
* End the kbd grab, ie restore the original keyboard handler
**********************************************************************/
void gr_end_kbd_grab(void);

/**********************************************************************
* Defines for the scan codes of some keys. See others from
* some book, for example Peter Norton's Programmers guide or
* write a program to try them out...
**********************************************************************/

#define GR_KEY_ESC  1
#define GR_KEY_1    2
#define GR_KEY_2    3
#define GR_KEY_3    4
#define GR_KEY_4    5
#define GR_KEY_5    6
#define GR_KEY_6    7
#define GR_KEY_7    8
#define GR_KEY_8    9
#define GR_KEY_9    10
#define GR_KEY_0    11

#define GR_KEY_TAB  15
#define GR_KEY_Q    16
#define GR_KEY_W    17
#define GR_KEY_E    18
#define GR_KEY_R    19 
#define GR_KEY_T    20
#define GR_KEY_Y    21
#define GR_KEY_U    22
#define GR_KEY_I    23
#define GR_KEY_O    24
#define GR_KEY_P    25

#define GR_KEY_A    30
#define GR_KEY_S    31
#define GR_KEY_D    32
#define GR_KEY_F    33
#define GR_KEY_G    34
#define GR_KEY_H    35
#define GR_KEY_J    36
#define GR_KEY_K    37
#define GR_KEY_L    38

#define GR_KEY_Z    44
#define GR_KEY_X    45 
#define GR_KEY_C    46
#define GR_KEY_V    47
#define GR_KEY_B    48
#define GR_KEY_N    49
#define GR_KEY_M    50
#define GR_KEY_COMMA    51
#define GR_KEY_DOT      52

#define GR_KEY_SPACE    57

#define GR_KEY_ARROW_UP     72
#define GR_KEY_ARROW_DOWN   80
#define GR_KEY_ARROW_LEFT   75
#define GR_KEY_ARROW_RIGHT  77

#endif
