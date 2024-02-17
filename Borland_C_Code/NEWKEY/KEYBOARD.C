		#pragma inline

		#include <dos.h>
		#include <conio.h>
		#include <stdio.h>
		#include <stdlib.h>

		#include "keyboard.h"

		void static interrupt (*OldInt9)(void);   // Old BIOS function
		extern unsigned char ScanCode;            // Scan code sent by Keyboard
		extern unsigned char KeyTable[128];       // Table to track scancodes
		extern unsigned char KeyboardStatus;      // Hold LED status

		unsigned char OldKeyboardStatus;          // Holds old LED status
		void static interrupt (*OldInt9)(void);   // Old BIOS function


//
//  New keyboard (Interrupt 9h) handler
//

static void interrupt NewInt9(void)
{

asm {
		mov dx,60h         // Read Scancode
		in	al,dx          // from keyboard port
		mov ScanCode,al    // Save original scan code

		mov bl,al          //
		mov bh,0           // Get offset pointer into KeyTable[]
		and bx,127         //
		test	al,10000000b // Is this a key-released scancode
		jz	leavealone     // No. Let it pass
		xor	al,al          // Yes! Zero it to clear KeyTable entry
		}
leavealone:
asm {
		mov byte ptr KeyTable[bx],al  // Save or clear KeyTable[ScanCode]

		mov dx,61h    // Read keyboad status port
		in	al,dx
		mov	bl,al

		or	al,080h   // Write with bit 7 on
		out	dx,al     // Clearing keyboard

		mov	al,bl     // Write with bit 7 off
		out	dx,al     // Enabling keyboard

		mov dx,20h    // Clear/Reset the PIC
		mov al,20h    // so we can do it again
		out	dx,al
		}
}

void RestoreKeyboardInt(void)
{
	unsigned char Byte;

	Byte = (OldKeyboardStatus >> 4) & 7;  // Prepare Old Status Byte
	KeyboardCommand(0xed, Byte);  // Restore old Status byte
	setvect(0x09,OldInt9);   // restore old keyboard interrupt
}

void InstallKeyboardInt(void)
{
	OldInt9=getvect(0x09);   // save old keyboard interrupt
	setvect(0x09,NewInt9);   // install our new handler
	OldKeyboardStatus = *((char far *)0x00000417);  // Save Status Byte
	KeyboardCommand(0xed, KeyboardStatus); // We start out with no LEDs lit
}



int WaitClear(void)            // Wait until keyboard is cleared
{
	unsigned int timeout=0;      // We will time out after 0xffff reads

	while(inp(0x64)&2)           // Wait until Input Register bit cleared
	{
		if(!++timeout) return 1;   // Oh Oh!
	}
	return 0;                    // Its cleared
}

int WaitForACK(void)           // Port 0x60 wait for ACK
{
	unsigned int timeout=0;      // Lots of waiting time

	while((inp(0x64)&1)!=1)      // Do we have data ready at PORT 60h
	{                            // Bit 0 of PORT 64h tells us that!
		if(!++timeout) return 1;   // Timed out error
	}
	if(inp(0x60)==0xfa)
		return 0;                  // ACK recieved
	else
		return 1;
}

void KeyboardCommand(char Command, char Data)  // Send command to Keyboard
{
		if(WaitClear()) TimeOutError(); // Wait til ready
		disable(); // We don't want to be interupted
		outportb(0x60, Command);         // Send command
		if(WaitForACK()) TimeOutError(); // and wait for ACK
		outportb(0x60, Data);            // Send Command Data
		if(WaitForACK()) TimeOutError(); // and wait for ACK
		enable(); // Interrupts ok.
}

void TimeOutError(void)  // Keyboard timeout error to screen.
{
	ScanCode=inp(0x60);  // Read Data Output port. Might fix Keyboard LOCK.
	printf("Keyboard error.\n");
	exit(1);
}



