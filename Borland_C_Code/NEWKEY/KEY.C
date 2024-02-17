
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include "keyboard.h"

#define TRUE 1
#define FALSE 0
#define KeyOn  0xdf
#define KeyOff 0xc4
#define LIGHT  0xdc


unsigned char ScanCode;         // Scan code of the last key pressed
unsigned char KeyTable[128];    // Table for holding key presses
unsigned char KeyboardStatus=0; // Hold LED status

struct KEYS{
		int On;    // Keeps track of whats presently pressed
		int X;     // X location this key is on the screen
		int Y;     // Y location this key is on the screen
		int Next;  // Points to a FAKE scancode of a second
};             // key such as the second [ENTER] key.

struct KEYS ScanLocs[104] =
{ //                       Scan   Key
	//     On   X   Y  Next  Code   Pressed
	//     --  --  --  ----  ----   --------------------
				{ 0,  0,  0,  0 }, // 0   (NUL and never used)
				{ 0,  8,  2,  0 }, // 1   (ESC)
				{ 0, 11,  5,  0 }, // 2   1
				{ 0, 14,  5,  0 }, // 3   2
				{ 0, 17,  5,  0 }, // 4   3
				{ 0, 20,  5,  0 }, // 5   4
				{ 0, 23,  5,  0 }, // 6   5
				{ 0, 26,  5,  0 }, // 7   6
				{ 0, 29,  5,  0 }, // 8   7
        { 0, 32,  5,  0 }, // 9   8
        { 0, 35,  5,  0 }, // 10  9
        { 0, 38,  5,  0 }, // 11  0
				{ 0, 41,  5,  0 }, // 12  -
        { 0, 44,  5,  0 }, // 13  =
				{ 0, 50,  5,  0 }, // 14  (BACKSPACE)
        { 0,  9,  7,  0 }, // 15  (TAB)
				{ 0, 12,  7,  0 }, // 16  Q
				{ 0, 15,  7,  0 }, // 17  W
				{ 0, 18,  7,  0 }, // 18  E
        { 0, 21,  7,  0 }, // 19  R
        { 0, 24,  7,  0 }, // 20  T
				{ 0, 27,  7,  0 }, // 21  Y
	{ 0, 30,  7,  0 }, // 22  U
        { 0, 33,  7,  0 }, // 23  I
				{ 0, 36,  7,  0 }, // 24  O
				{ 0, 39,  7,  0 }, // 25  P
        { 0, 42,  7,  0 }, // 26  [
				{ 0, 45,  7,  0 }, // 27  ]
        { 0, 49,  9, 90 }, // 28  (ENTER)
        { 0,  9, 13, 91 }, // 29  (CTRL)
        { 0, 13,  9,  0 }, // 30  A
				{ 0, 16,  9,  0 }, // 31  S
        { 0, 19,  9,  0 }, // 32  D
				{ 0, 22,  9,  0 }, // 33  F
        { 0, 25,  9,  0 }, // 34  G
				{ 0, 28,  9,  0 }, // 35  H
        { 0, 31,  9,  0 }, // 36  J
				{ 0, 34,  9,  0 }, // 37  K
        { 0, 37,  9,  0 }, // 38  L
        { 0, 40,  9,  0 }, // 39  ;
        { 0, 43,  9,  0 }, // 40  '
				{ 0,  8,  5,  0 }, // 41  `
        { 0,  9, 11,  0 }, // 42  (LEFT SHIFT)
        { 0, 47,  5,  0 }, // 43  \
				{ 0, 14, 11,  0 }, // 44  Z
        { 0, 17, 11,  0 }, // 45  X
				{ 0, 20, 11,  0 }, // 46  C
        { 0, 23, 11,  0 }, // 47  V
        { 0, 26, 11,  0 }, // 48  B
        { 0, 29, 11,  0 }, // 49  N
				{ 0, 32, 11,  0 }, // 50  M
        { 0, 35, 11,  0 }, // 51  ,
				{ 0, 38, 11,  0 }, // 52  .
				{ 0, 41, 11,103 }, // 53  /
				{ 0, 47, 11,  0 }, // 54  (RIGHT SHIFT)
				{ 0, 70,  5,  0 }, // 55  (GREY *)
				{ 0, 16, 13, 92 }, // 56  (ALT)
	{ 0, 29, 13,  0 }, // 57  (SPACE BAR)
        { 0,  9,  9,  0 }, // 58  (CAPS)
        { 0, 15,  2,  0 }, // 59  (F1)
        { 0, 18,  2,  0 }, // 60  (F2)
				{ 0, 21,  2,  0 }, // 61  (F3)
        { 0, 24,  2,  0 }, // 62  (F4)
				{ 0, 28,  2,  0 }, // 63  (F5)
				{ 0, 31,  2,  0 }, // 64  (F6)
				{ 0, 34,  2,  0 }, // 65  (F7)
        { 0, 37,  2,  0 }, // 66  (F8)
        { 0, 41,  2,  0 }, // 67  (F9)
        { 0, 44,  2,  0 }, // 68  (F10)
				{ 0, 64,  5,  0 }, // 69  (NUM LOCK)
        { 0, 57,  2,  0 }, // 70  (SCROLL LOCK)
				{ 0, 64,  7, 93 }, // 71  (HOME)
				{ 0, 67,  7, 94 }, // 72  (UP ARROW)
				{ 0, 70,  7, 95 }, // 73  (PAGE UP)
        { 0, 73,  5,  0 }, // 74  (GREY MINUS)
				{ 0, 64,  9, 96 }, // 75  (LEFT ARROW)
        { 0, 67,  9,  0 }, // 76  (KEYPAD 5)
        { 0, 70,  9, 97 }, // 77  (RIGHT ARROW)
        { 0, 73,  9,  0 }, // 78  (GREY PLUS)
        { 0, 64, 11, 98 }, // 79  (END)
        { 0, 67, 11, 99 }, // 80  (DOWN ARROW)
				{ 0, 70, 11, 100}, // 81  (PAGE DOWN)
				{ 0, 66, 13, 101}, // 82  (INSERT)
				{ 0, 70, 13, 102}, // 83  (DELETE)
				{ 0, 54,  2,  0 }, // 84  (ALT PRINT SCREEN)
				{ 0,  1,  1,  0 }, // 85
				{ 0,  1,  1,  0 }, // 86
				{ 0, 47,  2,  0 }, // 87  (F11)
				{ 0, 50,  2,  0 }, // 88  (F12)
				{ 0,  1,  1,  0 }, // 89

				// These next scan codes do not exist. They are used here to point
				// to keys on my KEYBOARD TEMPLATE only.

				{ 0, 73, 13,  0 }, // 90  (FAR RIGHT ENTER)
				{ 0, 49, 13,  0 }, // 91  (RIGHT CRTL)
				{ 0, 42, 13,  0 }, // 92  (RIGHT ALT)
				{ 0, 57,  5,  0 }, // 93  (GREY HOME)
				{ 0, 57, 11,  0 }, // 94  (GREY UP ARROW)
				{ 0, 60,  5,  0 }, // 95  (GREY PAGEUP)
				{ 0, 54, 13,  0 }, // 96  (GREY LEFT ARROW)
				{ 0, 60, 13,  0 }, // 97  (GREY RIGHT ARROW)
				{ 0, 57,  7,  0 }, // 98  (GREY END)
				{ 0, 57, 13,  0 }, // 99  (GREY DOWN ARROW)
				{ 0, 60,  7,  0 }, // 100 (GREY PAGE DOWN)
				{ 0, 54,  5,  0 }, // 101 (GREY INSERT)
				{ 0, 54,  7,  0 }, // 102 (GREY DELETE)
				{ 0, 67,  5,  0 }  // 103 (GREY SLASH /)
};

void	main(void)
{

	int done=FALSE, n;
	unsigned char OldKey=0;    // Used to compare old and new keys

//	_setcursortype(_NOCURSOR); // Turn off the cursor
  //	clrscr();
	InstallKeyboardInt();

	while(!done)
	{
		if(OldKey != ScanCode)
		{
			OldKey=ScanCode;
			DoKeys();
			if(ScanCode==1)
			{
				done=1;
				while(ScanCode==1);
			}
		}
	}

	RestoreKeyboardInt();  // Give old Keyboard ISR back to MessyDos
	_setcursortype(_NORMALCURSOR); /* Switch back to the normal cursor */
	clrscr(); // Clear our garbage off screen

}

void DoKeys(void)
{

	int n, nextone;

	for(n = 1; n < 90; n++)
	{
		if(KeyTable[n]!=ScanLocs[n].On)
		{
			nextone=ScanLocs[n].Next;

			if(KeyTable[n])
			{
			      //	textcolor(12);
			     //	gotoxy(ScanLocs[n].X, ScanLocs[n].Y);
			     //	cprintf("%c",ScanLocs[n]);
				if(nextone)
				{
				     //	gotoxy(ScanLocs[nextone].X, ScanLocs[nextone].Y);
					printf("%c",KeyOn);
					printf("%d",KeyOn);
				}
			}
			else
			{
			       //	textcolor(15);
			      //	gotoxy(ScanLocs[n].X, ScanLocs[n].Y);
			      //	cprintf("%c",ScanLocs[n].X);
				if(nextone)
				{
				     //	gotoxy(ScanLocs[nextone].X, ScanLocs[nextone].Y);
				      //	cprintf("%c",KeyOff);
				}
			}
			ScanLocs[n].On=KeyTable[n];
		}
	}
}
