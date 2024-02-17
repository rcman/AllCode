'Draw Image by Sean Godsell

DIM img%(1 TO 1000, 1 TO 32)

SCREEN 9, 1, 0, 0

' Init variables
yf = 8
yo = 25
xf = 10
xo = 70
cont = 0
image = 1
c = 7

LINE (xo - 1, yo - 1)-(xo - 1, 350), 15
LINE (xo - 1, yo - 1)-(640, yo - 1), 15
LINE (0, 0)-(50, 50), 15, B
GOSUB printspcs
grid = 0
sc = POINT(x% * xf + xo, y% * yf + yo)

' Main routine to draw the flashing block cursor and get input
drawship:
        LINE (x% * xf + xo, y% * yf + yo)-(x% * xf + xo + xf - 1, y% * yf + yo + yf - 1), INT(RND(1) * 16), BF
        a$ = INKEY$: IF a$ = "" THEN GOTO drawship
        LINE (x% * xf + xo, y% * yf + yo)-(x% * xf + xo + xf - 1, y% * yf + yo + yf - 1), sc, BF

        IF a$ = "4" OR a$ = "7" OR a$ = "1" THEN x% = x% - 1: IF x% < 0 THEN x% = 0
        IF a$ = "6" OR a$ = "9" OR a$ = "3" THEN x% = x% + 1
        IF a$ = "2" OR a$ = "1" OR a$ = "3" THEN y% = y% + 1
        IF a$ = "8" OR a$ = "7" OR a$ = "9" THEN y% = y% - 1: IF y% < 0 THEN y% = 0
        IF a$ = "Q" OR a$ = "q" THEN END
        IF a$ = "-" THEN GOSUB saveimage: image = image - 1: IF image < 1 THEN image = 1: GOSUB restimage ELSE GOSUB restimage
        IF a$ = "+" THEN GOSUB saveimage: image = image + 1: IF image > 32 THEN image = 32: GOSUB restimage ELSE GOSUB restimage
        IF a$ = "S" OR a$ = "s" THEN GOSUB savedata
        IF a$ = "R" OR a$ = "r" THEN GOSUB restdata
        IF a$ = "O" OR a$ = "o" THEN GOSUB restimage
        IF a$ = "V" OR a$ = "v" THEN GOSUB changeview
        IF a$ = "G" OR a$ = "g" THEN GOSUB gotoimage
        IF a$ = "F" OR a$ = "f" THEN GOSUB flipimage
        IF a$ = "C" OR a$ = "c" THEN GOSUB copyimage
        IF a$ = "E" OR a$ = "e" THEN LINE (1, 1)-(45, 35), 0, BF: GOSUB saveimage
        IF a$ = "D" OR a$ = "d" THEN cont = cont + 1: IF cont > 1 THEN cont = 0
        IF a$ = " " OR cont = 1 THEN LINE (x% * xf + xo, y% * yf + yo)-(x% * xf + xo + xf - 1, y% * yf + yo + yf - 1), c, BF: PSET (x% + 1, y% + 1), c
        IF a$ = "," THEN c = c - 1: IF c < 0 THEN c = 15
        IF a$ = "." THEN c = c + 1: IF c > 15 THEN c = 0
        GOSUB printspcs
        sc = POINT(x% * xf + xo, y% * yf + yo)
        GOTO drawship

' Display the menu items and current settings
printspcs:
        LOCATE 10, 1: PRINT "COLOUR"
        LINE (8, 150)-(36, 175), c, BF
        LOCATE 15, 1: PRINT "X:"; x%
        LOCATE 17, 1: PRINT "Y:"; y%
        LOCATE 19, 1: PRINT "IMAGE"
        PRINT " "; image
        LOCATE 22, 1: IF cont = 1 THEN PRINT "DRAW" ELSE PRINT "    "
        LOCATE 1, 10: PRINT "Keys (S)ave (R)est (+/-) Next/Prev Image (C)opy Image (G)rid"
        RETURN

' Save the current image element
saveimage:
        GET (1, 1)-(45, 35), img%(1, image)
        RETURN

' Restore and display and image element
restimage:
        LINE (1, 1)-(45, 35), 0, BF
        IF img%(1, image) <> 0 THEN PUT (1, 1), img%(1, image)
        FOR I = 1 TO 45
                FOR J = 1 TO 33
                        p = POINT(I, J)
                        LINE ((I - 1) * xf + xo, (J - 1) * yf + yo)-((I - 1) * xf + xo + xf - 1, (J - 1) * yf + yo + yf - 1), p, BF
                NEXT J
        NEXT I
        RETURN

' Copy the current image to another image
copyimage:
        LOCATE 1, 10: PRINT "                                                                 "
        LOCATE 1, 10: PRINT "Copy from Image "; image;
        INPUT " to : ", I
        IF I < 1 OR I > 32 THEN RETURN
        simage = image
        image = I
        GOSUB saveimage
        image = simage
        RETURN

' Save image data to a file
savedata:
        LOCATE 1, 10: PRINT "                                                                  "
        LOCATE 1, 10: INPUT "Enter Filename to Save to : ", n$
        IF n$ = "" THEN RETURN
        DEF SEG = VARSEG(img%(1, 1))
        BSAVE n$, VARPTR(img%(1, 1)), (1000 * 32)
        DEF SEG
        RETURN

' Open image file and restore
restdata:
        LOCATE 1, 10: PRINT "                                                                  "
        LOCATE 1, 10: INPUT "Enter Filename to Restore from : ", n$
        IF n$ = "" THEN RETURN
        DEF SEG = VARSEG(img%(1, 1))
        BLOAD n$, VARPTR(img%(1, 1))
        DEF SEG
        GOSUB restimage
        RETURN

gotoimage:
        GOSUB saveimage
        LOCATE 1, 10: PRINT "                                                                  "
        LOCATE 1, 10: INPUT "Enter Image to go to : ", imgn: IF imgn < 1 OR imgn > 32 THEN RETURN
        image = imgn
        GOSUB restimage
        RETURN

flipimage:
        LOCATE 1, 10: PRINT "                                                                  "
        LOCATE 1, 10: INPUT "Flip Image X or Y : ", n$
        n$ = UCASE$(n$): IF n$ <> "X" AND n$ <> "Y" THEN RETURN
        IF n$ = "Y" THEN GOTO flipy
        FOR J = 1 TO 33
                x1 = 1
                x2 = 45
                FOR I = 1 TO 45 / 2
                        p1 = POINT(x1, J)
                        p2 = POINT(x2, J)
                        PSET (x1, J), p2
                        PSET (x2, J), p1
                        x1 = x1 + 1
                        x2 = x2 - 1
                NEXT I
        NEXT J
        GOSUB saveimage
        GOSUB restimage
        RETURN
flipy:
        FOR I = 1 TO 45
                y1 = 1
                y2 = 33
                FOR J = 1 TO 33 / 2
                        p1 = POINT(I, y1)
                        p2 = POINT(I, y2)
                        PSET (I, y1), p2
                        PSET (I, y2), p1
                        y1 = y1 + 1
                        y2 = y2 - 1
                NEXT J
        NEXT I
        GOSUB saveimage
        GOSUB restimage
        RETURN

changeview:
        
        SCREEN 9, 1, 1, 1
        CLS
        PRINT "DRAWING"
        c1 = 10: C2 = 10: im = 1
        INPUT "Enter Number to display to"; in
        CLS

KEEPDRAWING:   
        LOCATE 1, 1: PRINT "Image #"; im
        PUT (c1, C2), img%(1, im)
        c1 = c1 + 50
        IF c1 > 500 THEN c1 = 10: C2 = C2 + 40
        IF C2 > 300 THEN GOTO k1
        im = im + 1
        IF im > in THEN GOTO k1
        FOR T = 1 TO 4000: NEXT T

        GOTO KEEPDRAWING


k1:
        c$ = INKEY$: IF c$ = "" THEN GOTO k1
        SCREEN 9, 1, 0, 0

        RETURN

