
#include <memory.h>
#include <alloc.h>
#include <stdio.h>
#include <process.h>
#include "draw.h"


// Prototype

void Alloc_Vert_Screen(void);
void Free_Vert_Screen(void);

int VirStart;

char far *dest;

void Alloc_Vert_Screen(void)
{

	dest = (char far *) farmalloc(64000l);

	if (dest == NULL)
	{
		 printf("Not enough memory to allocate screen buffer. Press any key to Exit \n");
		 exit(1);  /* terminate program if out of memory */
	}
	_fmemset(dest, 0, 64000l);


	// VirStart = (int)dest;

}

void Free_Vert_Screen(void)
{

	farfree(dest);

}
