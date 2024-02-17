' Shooting Routine

'DIM Sht(x,y,n)
DIM sht(10, 10, 80)
DIM Box%(1 TO 200)
DIM shp1%(200)
DIM shp2%(200)
SCREEN 1

DATA 0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0
DATA 0,3,3,2,1,2,1,2,1,2,1,2,1,0,0,0,0
DATA 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,0
DATA 3,3,3,2,1,2,1,2,1,2,1,2,1,0,0,0,0
DATA 0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0

DATA 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0
DATA 0,0,0,0,1,2,1,2,1,2,1,2,1,2,3,3,0
DATA 0,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3
DATA 0,0,0,0,1,2,1,2,1,2,1,2,1,2,3,3,3
DATA 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0

x1% = 0: x2% = 3: y1% = 0: y2% = 0    'Box start X and start Y
a1% = 0: a2% = 10: b1% = 1: b2% = 5   'Box start X and start Y
LINE (0, 0)-(3, 0), 2, BF       'Draw Box with Box Fill
GET (x1%, y1%)-(x2%, y2%), Box%         ' get the shoot


CLS

FOR y = 0 TO 4
FOR x = 0 TO 16
        READ pix
        PSET (x, y), pix
NEXT x
NEXT y

GET (0, 0)-(16, 4), shp1%   ' get the ship

CLS
FOR y = 0 TO 4
FOR x = 0 TO 16
        READ pix
        PSET (x, y), pix
NEXT x
NEXT y

GET (0, 0)-(16, 4), shp2%   ' get the ship

x = 160: y = 15: sy = 15
ship = 1
smv = 4
PUT (x, sy), shp1%


main:
        FOR i = 1 TO 50: NEXT i

        GOSUB moveship
        GOSUB moveshots
        GOTO main


' Subroutine to move your ship across the heavens
moveship:
        a$ = INKEY$: IF a$ = "" THEN GOTO gohere
        IF ship = 1 THEN PUT (x, sy), shp1%, XOR
        IF ship = 2 THEN PUT (x, sy), shp2%, XOR
        IF a$ = "2" THEN sy = sy + 4
        IF a$ = "8" THEN sy = sy - 4
        IF a$ = "4" THEN ship = 2: x = x - 4: smv = -4
        IF a$ = "6" THEN ship = 1: x = x + 4: smv = 4
        IF a$ = "z" THEN GOSUB fireshot
        IF ship = 1 THEN PUT (x, sy), shp1%, XOR
        IF ship = 2 THEN PUT (x, sy), shp2%, XOR
gohere:
        RETURN


'move shot across the screen
moveshots:
        FOR sh = 1 TO 40
        IF sht(1, 1, sh) = 0 THEN GOTO sap
        
        nx = sht(1, 1, sh)
        ny = sht(1, 2, sh)
        IF nx > 310 OR nx < 10 THEN sht(1, 1, sh) = 0: PUT (nx, ny + 2), Box%, XOR: GOTO sap
       
       
        PUT (nx, ny + 2), Box%, XOR
       
        nx = nx + sht(1, 3, sh)
        sht(1, 1, sh) = nx
       
        PUT (nx, ny + 2), Box%
sap:
        NEXT sh
       
        RETURN

' Subroutine to fire shot from your ship
fireshot:
        FOR i = 1 TO 40
        IF sht(1, 1, i) = 0 THEN GOTO ov
        NEXT i
        RETURN
ov:
        sht(1, 1, i) = x
        sht(1, 2, i) = sy: PUT (x, sy + 2), Box%
        sht(1, 3, i) = smv
      
       
        RETURN

