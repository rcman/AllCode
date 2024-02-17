/**********************************************************************
* gr.c
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

**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>
#include <conio.h>
#include <graphics.h>
#include <dos.h>



/**********************************************************************
* Array of booleans for each key of the keyboard (indexed by the scan
* code value). Non-zero if key pressed, zero otherwise.
* The array is updated during the kbd_grab, see the function
* gr_start_kbd_grab below.
**********************************************************************/
char gr_keys[128];


/** Vector to the old keyboard interrupt handler **/
void interrupt (*gr_old_int9)(void);

static unsigned int far *fp_kbd_tail, far *fp_kbd_head;

/** The new keyboard interrupt handler, see start_kbd_grab() in gr.c **/
void interrupt gr_int9(void)
{
    unsigned char a;

    a = inp(0x60);              /** read key code and (re)set it to table **/
    if (a & 0x80)
        gr_keys[a & 0x7F] = 0;
    else
        gr_keys[a] = 1;

    fp_kbd_tail = MK_FP(0x40,0x1A);
    fp_kbd_head = MK_FP(0x40,0x1C);
    *fp_kbd_tail = *fp_kbd_head;          /** flush keyboard buffer **/

    gr_old_int9();                 /** allow old handler (eg breakon.com) **/
}










/**********************************************************************
* Restore the original keyboard handler
**********************************************************************/
void gr_end_kbd_grab(void)
{
    setvect(9, gr_old_int9);
}

/**********************************************************************
* Set a new handler for the keyboard. This handler sets and resets
* the gr_keys array values for each scancode received, flushes the
* keyboard buffer and then calls the original handler.
**********************************************************************/
void gr_start_kbd_grab(void)
{
    static int first_time = 1;

    gr_old_int9 = getvect(9);
    memset(gr_keys, 0, sizeof(gr_keys));
    setvect(9, gr_int9);
    if (first_time) {
        atexit(gr_end_kbd_grab);
        first_time = 0;
    }
}