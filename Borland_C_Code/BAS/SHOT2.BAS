' Shooting Routine

'DIM Sht(x,y,n)
DIM sht(10, 10, 80)

DIM Box%(1 TO 200)
DIM shp%(1 TO 200)
SCREEN 1
x1% = 0: x2% = 3: y1% = 0: y2% = 0    'Box start X and start Y
a1% = 0: a2% = 3: b1% = 0: b2% = 0    'Box start X and start Y
LINE (a1%, b1%)-(a2%, b2%), 2, BF       'Draw Box with Box Fill
LINE (x1%, y1%)-(x2%, y2%), 2        'Draw Box with Box Fill
GET (x1%, y1%)-(x2%, y2%), Box%         ' get the shoot
GET (a1%, b1%)-(a2%, b2%), shp%         ' get the ship
x = 39: y = 15: sy = 15
shtnum = 1

frsht = 1

main:
        GOSUB moveship
        GOSUB moveshots
        GOSUB seeship
        GOTO main

moveship:
        a$ = INKEY$
        IF a$ = "2" THEN sy = sy + 2
        IF a$ = "8" THEN sy = sy - 2
        IF a$ = "z" THEN GOSUB fireshot
        RETURN

seeship:
       
        PUT (x, sy), shp%, XOR
        PUT (x, sy), shp%
        RETURN


moveshots:
       
        FOR sh = 1 TO 20
        IF sht(1, 1, sh) = 0 THEN GOTO sap
        nx = sht(1, 1, sh)
        IF nx > 240 THEN sht(1, 1, sh) = 0: PUT (nx, ny), Box%, XOR: GOTO sap
        nx = sht(1, 1, sh)
        ny = sht(1, 2, sh)
        PUT (nx, ny), Box%, XOR
        nx = nx + 1
        sht(1, 1, sh) = nx
        PUT (nx, ny), Box%
sap:
        NEXT sh
        RETURN

fireshot:
        FOR i = 1 TO 20
        IF sht(1, 1, i) = 0 THEN sht(1, 1, i) = x: sht(1, 2, i) = sy: GOTO ov
        NEXT i
ov:      
        RETURN

