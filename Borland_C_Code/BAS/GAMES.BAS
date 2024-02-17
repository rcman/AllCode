
'rem Start of basic system

DIM a(10, 20)         ' 200 shots and 200 hor. positions
DIM b(10, 20)
SCREEN 9
v = 10: h = 10
count = 1
vr = 1
LOCATE v, h: PRINT ">"  ' ship will be > symbol
start:
LOCATE 1, 1: PRINT count
 count = count + 1


a$ = INKEY$
LOCATE v, h: PRINT ">"
IF a$ = CHR$(32) THEN GOTO fire
IF a$ = "a" THEN GOTO up
IF a$ = "z" THEN GOTO down
IF a$ = "" THEN GOTO keepshooting


fire:

        s = 1
        IF vr = 19 THEN GOTO start
        ' a$ (200, 200)        ' 200 shots and 200 hor. positions
        ' shot on screen
        LOCATE v, h: PRINT " - "
        'PRINT v, h
        a(vr, 1) = v
        b(vr, 1) = h
        PRINT a(vr, 1)
        PRINT b(vr, 1)
        PRINT vr
        
        vr = vr + 1
up:
        IF v < 1 THEN h = 1
        LOCATE v, h: PRINT " "
        v = v - 1
        GOTO start

down:
        IF v > 21 THEN h = 21
        LOCATE v, h: PRINT " "
        v = v + 1
        GOTO start

keepshooting:

         IF s = 0 THEN GOTO start
         FOR i = 1 TO 20
         j = a(i, 1)
         k = b(i, 1)
        '        PRINT j, k
        IF k > 60 THEN k(1, 1) = 0: s = 0: vr = vr - 1

        ' LOCATE j, k: PRINT " - "

         k = k + 1
         b(i, 1) = k
         
        NEXT

        ' vr = vr + 1
         GOTO start

