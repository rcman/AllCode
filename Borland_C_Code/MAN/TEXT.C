
#include <conio.h>
#include <stdio.h>

#pragma inline

void ptext(void)
{


    clrscr();
    cprintf("\r\n");
    textcolor(4);
    cprintf("Draw 96\r\n");
    cprintf("Copyright 1996(C)\r\n");
    textcolor(11);
    cprintf("Program Written By:\r\n");
    cprintf("Franco Gaetan & Sean Godsell\r\n");
    cprintf("This Program is NOT FREE\r\n");
    cprintf("Enter Password:");
    getch();



}

void Init_Mode(void)
{
        printf("Initialize 320 x 200 256 Colours\n");
        asm {
                mov ax,0x13
                int 0x10
             }
}

void Close_Mode()
{

        asm {
                mov ax,0x03
                int 0x10
                }
        printf("Graphics Screen Closed Thank You\n");

}

