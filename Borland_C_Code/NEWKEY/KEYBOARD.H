#ifndef __KEYBOARD_H
#define __KEYBOARD_H


// User usable functions

// functions in KEYBOARD.C

void RestoreKeyboardInt(void); // Replaces the old keyboard IRS with ours
void InstallKeyboardInt(void); // Reinstates the old keyboard ISR
int WaitClear(void);           // Wait until keyboard is cleared
int WaitForACK(void);          // Port 0x60 wait for ACK
void KeyboardCommand(char Command, char Data); // Send command to keyboard
void TimeOutError(void);       // Keyboard timeout error to screen
															 // most likely the keyboard totally locked
															 // up too!


// functions in TEMPLATE.C

void BuildTemplate(void); // Puts funky keyboard template on screen
void DoKeys(void);        // Parses the keyboards IRS key table and gives                                                                                                                // visual output of the results.
void FlipLites(int Led);  // Puts LEDs on the screen template.


#endif
