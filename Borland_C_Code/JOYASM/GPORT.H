/*
// GPORT.H
//
// Copyright (c) 1991 Bri Productions
//
*/



/*
//-------------------------------------
//
// GamOpen valid axis bits
//
//-------------------------------------
*/

#define  JAX_VAL  0x1
#define  JAY_VAL  0x2
#define  JBX_VAL  0x4
#define  JBY_VAL  0x8
#define  JA_VAL   (JAX_VAL) | (JAY_VAL)
#define  JB_VAL   (JBX_VAL) | (JBY_VAL)



/*
//-------------------------------------
//
// joysticks
//
//-------------------------------------
*/

#define  JA    0
#define  JB    1


/*
//-------------------------------------
//
// joystick axes
//
//-------------------------------------
*/

#define  JAX    0
#define  JAY    1
#define  JBX    2
#define  JBY    3


/*
//-------------------------------------
//
// joystick buttons
//
//-------------------------------------
*/

#define  BA1_CURR   0x01
#define  BA2_CURR   0x02
#define  BB1_CURR   0x04
#define  BB2_CURR   0x08
#define  BA1_PEND   0x10
#define  BA2_PEND   0x20
#define  BB1_PEND   0x40
#define  BB2_PEND   0x80



/*
//-------------------------------------
//
// centering modes
//
//-------------------------------------
*/

#define MEAN   1
#define REAL   0



/*
//-------------------------------------
//
// byte typedef
//
//-------------------------------------
*/

#ifndef BYTE
#define BYTE
typedef unsigned char byte;
#endif




/*
//-------------------------------------
//
// function prototypes
//
//-------------------------------------
*/

int      GamOpen     (void);
void     GamClose    (void);
signed   GamAxis     (byte axis);
void     GamAxes     (byte stick, signed *x, signed *y);
unsigned GamRawAxis  (byte axis);
void     GamCalAxis  (byte axis, unsigned raw_value);
byte     GamButton   (void);
void     GamCenter   (byte mode);
