'Draw Image by Sean Godsell

DIM img%(1 TO 500, 1 TO 32)
DIM m%(30)
ON ERROR GOTO ErrRoutine

SCREEN 9

' Init variables
yf = 8
yo = 25
xf = 10
xo = 70
cont = 0
image = 1
c = 7
dsp = 1

' Prepare
FOR i = 1 TO 32
        img%(i * 2, 32) = 47
        img%(i * 2 + 1, 32) = 35
NEXT i

DEF SEG = VARSEG(m%(0))
FOR i% = 0 TO 32
        READ d%
        POKE VARPTR(m%(0)) + i%, d%
NEXT i%
DATA 184,3,0
DATA 187,3,0
DATA 205,51
DATA 46,137,22,34,0
DATA 46,137,14,36,0
DATA 184,5,0
DATA 187,5,0
DATA 205,51
DATA 46,163,38,0
DATA 203,0,0

FOR i = 0 TO 7
        LINE (5, 75 + i * 15)-(18, 84 + i * 15), i, BF
        LINE (32, 75 + i * 15)-(45, 84 + i * 15), i + 8, BF
NEXT i

LINE (xo - 1, yo - 1)-(xo - 1, 350), 15
LINE (xo - 1, yo - 1)-(640, yo - 1), 15
LINE (0, 0)-(50, 50), 15, B
GOSUB printspcs
grid = 0
sc = POINT(x% * xf + xo, y% * yf + yo)
DEF SEG = VARSEG(m%(0))
CALL ABSOLUTE(VARPTR(m%(0)))
DEF SEG
mx = m%(18) / 13
my = m%(17) / 9
myo = my
mxo = mx
xc1 = 0
yc1 = 4
gv = 1
GOSUB dspcolor

' Main routine to draw the flashing block cursor and get input
drawship:
        DEF SEG = VARSEG(m%(0))
        CALL ABSOLUTE(VARPTR(m%(0)))
        DEF SEG
        mx = m%(18) / 13 - 3
        my = m%(17) / 9
        mb = m%(19)
        IF mx <= -3 THEN mx = 0: GOSUB getcolor
        IF mx < 0 THEN mx = 0
        LINE (x% * xf + xo, y% * yf + yo)-(x% * xf + xo + xf - gv, y% * yf + yo + yf - gv), INT(RND(1) * 8) + 9, BF
        a$ = UCASE$(INKEY$): IF a$ = "" AND mx = mxo AND my = myo AND mb < 1 THEN GOTO drawship
        la = LEN(a$)
        ra$ = RIGHT$(a$, 1)
        LINE (x% * xf + xo, y% * yf + yo)-(x% * xf + xo + xf - gv, y% * yf + yo + yf - gv), sc, BF

        IF a$ = "4" OR a$ = "7" OR a$ = "1" THEN x% = x% - 1: IF x% < 0 THEN x% = 0
        IF a$ = "6" OR a$ = "9" OR a$ = "3" THEN x% = x% + 1
        IF a$ = "2" OR a$ = "1" OR a$ = "3" THEN y% = y% + 1
        IF a$ = "8" OR a$ = "7" OR a$ = "9" THEN y% = y% - 1: IF y% < 0 THEN y% = 0
        IF mx > mxo THEN x% = mx
        IF my > myo THEN y% = my
        IF mx < mxo THEN x% = mx
        IF my < myo THEN y% = my
        mxo = mx
        myo = my
        IF a$ = "Q" THEN END
        IF a$ = "-" THEN GOSUB saveimage: image = image - 1: IF image < 1 THEN image = 1: GOSUB restimage ELSE GOSUB restimage
        IF a$ = "+" THEN GOSUB saveimage: image = image + 1: IF image > 32 THEN image = 32: GOSUB restimage ELSE GOSUB restimage
        IF a$ = "S" THEN GOSUB savedata
        IF a$ = "O" THEN GOSUB restdata
        IF a$ = "R" THEN GOSUB restimage
        IF a$ = "J" THEN GOSUB gotoimage
        IF a$ = "F" THEN GOSUB flipimage
        IF a$ = "C" THEN GOSUB copyimage
        IF a$ = "V" THEN GOSUB viewimage
        IF a$ = "X" THEN GOSUB cutimage

        IF la = 2 AND ra$ = CHR$(72) THEN GOSUB scrollup
        IF la = 2 AND ra$ = CHR$(77) THEN GOSUB scrollleft
        IF la = 2 AND ra$ = CHR$(75) THEN GOSUB scrollright
        IF la = 2 AND ra$ = CHR$(80) THEN GOSUB scrolldown

        IF a$ = "G" THEN gv = gv + 1: GOSUB dogrid
        IF a$ = "E" THEN LINE (1, 1)-(45, 35), 0, BF: GOSUB saveimage: GOSUB restimage
        IF a$ = "D" THEN cont = cont + 1: IF cont > 1 THEN cont = 0
        IF a$ = " " OR cont = 1 OR mb = 1 THEN LINE (x% * xf + xo, y% * yf + yo)-(x% * xf + xo + xf - gv, y% * yf + yo + yf - gv), c, BF: PSET (x% + 1, y% + 1), c
        IF a$ = "," THEN c = c - 1: IF c < 0 THEN c = 15
        IF a$ = "." THEN c = c + 1: IF c > 15 THEN c = 0
        IF a$ = "." OR a$ = "," THEN GOSUB dspcolor
        GOSUB printspcs
        sc = POINT(x% * xf + xo + 1, y% * yf + yo + 1)
        GOTO drawship

' Get Selected Colour
getcolor:
        mxc = INT(m%(18) / 84)
        myc = INT(m%(17) / 44)
loopcolor:
        DEF SEG = VARSEG(m%(0))
        CALL ABSOLUTE(VARPTR(m%(0)))
        DEF SEG
        mx = INT(m%(18) / 84)
        my = INT(m%(17) / 44)
        mb = m%(19)
'        IF mx = mxc OR mx = myc AND mb < 1 THEN GOTO getcolor
        LINE (3 + mxc * 27, 74 + myc * 15)-(20 + mxc * 27, 85 + myc * 15), 0, B
        IF mx > 1 THEN GOSUB dspcolor: RETURN
        LINE (3 + mx * 27, 74 + my * 15)-(20 + mx * 27, 85 + my * 15), 15, B
        IF mb = 1 THEN c = mx * 8 + my: GOSUB cleancolor: GOSUB dspcolor: GOSUB waitmouse: RETURN
        mxc = mx
        myc = my
        GOTO getcolor

' Display the current colour
dspcolor:
        LINE (3 + xc1 * 27, 74 + yc1 * 15)-(20 + xc1 * 27, 85 + yc1 * 15), 0, B
        xc1 = INT(c / 8)
        yc1 = INT(c) - (xc1 * 8)
        LINE (3 + xc1 * 27, 74 + yc1 * 15)-(20 + xc1 * 27, 85 + yc1 * 15), 15, B
        RETURN

' Wait for mouse button depressed
waitmouse:
        DEF SEG = VARSEG(m%(0))
        CALL ABSOLUTE(VARPTR(m%(0)))
        DEF SEG
        mb = m%(19)
        IF mb <> 0 THEN GOTO waitmouse
        RETURN

' Clean up color spectrum
cleancolor:
FOR i = 0 TO 15
        xc1 = INT(i / 8)
        yc1 = INT(i) - (xc1 * 8)
        LINE (3 + xc1 * 27, 74 + yc1 * 15)-(20 + xc1 * 27, 85 + yc1 * 15), 0, B
NEXT i
        RETURN

' Display the menu items and current settings
printspcs:
        LOCATE 5, 1: PRINT "COLOUR"
        LINE (8, 200)-(36, 225), c, BF
        LOCATE 18, 1: PRINT "X:"; x%
        LOCATE 19, 1: PRINT "Y:"; y%
        LOCATE 21, 1: PRINT "IMAGE"
        PRINT " "; image
        LOCATE 23, 1: IF cont = 1 THEN PRINT "DRAW" ELSE PRINT "    "
        LOCATE 24, 1: PRINT img%(image * 2, 32); img%(image * 2 + 1, 32);
        LOCATE 1, 10: PRINT "Keys (S)ave (O)pen (+/-) Next/Prev (C)opy (G)rid (J)ump (D)raw (E)rase"
        RETURN

' Do the Grid and restore the image
dogrid:
        IF gv > 2 THEN gv = 1: LINE (xo, yo)-(xo + xf * 47, yo + yf * 35), 0, BF
        GOSUB saveimage
        IF gv = 1 THEN GOSUB restimage: RETURN
        GOSUB drawgrid
        RETURN

' Draw the actual grid on the screen
drawgrid:
        FOR i = 1 TO 35
                LINE (xo, i * yf + yo - 1)-(xo + xf * 47, i * yf + yo - 1), 15
        NEXT i
        FOR i = 1 TO 47
                LINE (i * xf + xo - 1, yo)-(i * xf + xo - 1, yo + yf * 35), 15
        NEXT i
        RETURN

' Save the current image element
saveimage:
        GET (1, 1)-(img%(image * 2, 32) + 1, img%(image * 2 + 1, 32) + 1), img%(1, image)
        RETURN

' Restore and display and image element
restimage:
        IF dsp THEN LINE (1, 1)-(47, 35), 0, BF
'        LINE (xo, yo)-(xo + xf * 45, yo + yf * 35), 0, BF
        IF img%(1, image) <> 0 AND dsp THEN PUT (1, 1), img%(1, image)
        FOR i = 1 TO 47
                FOR j = 1 TO 33
                        p = POINT(i, j)
                        LINE ((i - 1) * xf + xo, (j - 1) * yf + yo)-((i - 1) * xf + xo + xf - gv, (j - 1) * yf + yo + yf - gv), p, BF
                NEXT j
        NEXT i
        RETURN

' Copy the current image to another image
copyimage:
        LOCATE 1, 10: PRINT "                                                                 "
        LOCATE 1, 10: PRINT "Copy from Image "; image;
        INPUT " to : ", i
        IF i < 1 OR i > 32 THEN RETURN
        simage = image
        image = i
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
        LOCATE 1, 10: INPUT "Enter Image to jump to : ", imgn: IF imgn < 1 OR imgn > 32 THEN RETURN
        image = imgn
        GOSUB restimage
        RETURN

flipimage:
        LOCATE 1, 10: PRINT "                                                                  "
        LOCATE 1, 10: INPUT "Flip Image X or Y : ", n$
        n$ = UCASE$(n$): IF n$ <> "X" AND n$ <> "Y" THEN RETURN
        IF n$ = "Y" THEN GOTO flipy
        FOR j = 1 TO 33
                x1 = 1
                x2 = 47
                FOR i = 1 TO 47 / 2
                        p1 = POINT(x1, j)
                        p2 = POINT(x2, j)
                        PSET (x1, j), p2
                        PSET (x2, j), p1
                        x1 = x1 + 1
                        x2 = x2 - 1
                NEXT i
        NEXT j
        GOSUB saveimage
        GOSUB restimage
        RETURN
flipy:
        FOR i = 1 TO 47
                y1 = 1
                y2 = 33
                FOR j = 1 TO 33 / 2
                        p1 = POINT(i, y1)
                        p2 = POINT(i, y2)
                        PSET (i, y1), p2
                        PSET (i, y2), p1
                        y1 = y1 + 1
                        y2 = y2 - 1
                NEXT j
        NEXT i
        GOSUB saveimage
        GOSUB restimage
        RETURN

' View images on screen
viewimage:
        SCREEN 9, 1, 1
        CLS
        FOR i = 0 TO 3
                FOR j = 1 TO 8
                        IF img%(1, i * 8 + j) <> 0 THEN PUT (j * 70, (1 + i) * 70), img%(1, i * 8 + j)
                        LOCATE (i * 5) + 4, (j * 9): PRINT j + i * 8
                NEXT j
        NEXT i
        LOCATE 22, 1
waitkey:
        B$ = INKEY$: IF B$ = "" THEN GOTO waitkey
        CLS
        SCREEN 9, 1, 0
        RETURN

ErrRoutine:
        RESUME NEXT

scrollup:
        GOSUB saveimage
        LINE (1, 1)-(48, 45), 0, BF
        PUT (1, 0), img%(1, image)
        LINE (0, 0)-(50, 50), 15, B
        dsp = 0
        GOSUB restimage
        dsp = 1
        RETURN

scrolldown:
        GOSUB saveimage
        LINE (1, 1)-(48, 45), 0, BF
        PUT (1, 2), img%(1, image)
        LINE (0, 0)-(50, 50), 15, B
        dsp = 0
        GOSUB restimage
        dsp = 1
        RETURN

scrollright:
        GOSUB saveimage
        LINE (1, 1)-(48, 45), 0, BF
        PUT (0, 1), img%(1, image)
        LINE (0, 0)-(50, 50), 15, B
        dsp = 0
        GOSUB restimage
        dsp = 1
        RETURN

scrollleft:
        GOSUB saveimage
        LINE (1, 1)-(48, 45), 0, BF
        PUT (2, 1), img%(1, image)
        LINE (0, 0)-(50, 50), 15, B
        dsp = 0
        GOSUB restimage
        dsp = 1
        RETURN

' Cut the image to a selected size
cutimage:
        DEF SEG = VARSEG(m%(0))
        CALL ABSOLUTE(VARPTR(m%(0)))
        DEF SEG
        mx = m%(18) / 13 - 3
        my = m%(17) / 9
        mbut = m%(19)
        IF mx < 0 THEN mx = 0
        LINE (x% * xf + xo, y% * yf + yo)-(x% * xf + xo + xf - gv, y% * yf + yo + yf - gv), INT(RND(1) * 8) + 9, BF
        a$ = UCASE$(INKEY$): IF a$ = "" AND mx = mxc AND my = myc AND mb < 1 THEN GOTO drawship
        la = LEN(a$)
        ra$ = RIGHT$(a$, 1)
        LINE (x% * xf + xo, y% * yf + yo)-(x% * xf + xo + xf - gv, y% * yf + yo + yf - gv), sc, BF
        IF mbut = 1 THEN img%(image * 2, 32) = mx: img%(image * 2 + 1, 32) = my: RETURN
        mxc = mx
        myc = my
        GOTO cutimage


