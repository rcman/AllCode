
DIM img%(1 TO 500, 1 TO 32)            ' Images stored in .DAT file
DIM sht(10, 10, 80)

DIM enmy(4, 30)                         ' Enemy Array

DIM Box%(1 TO 200)
DIM shp1%(200)
DIM shp2%(200)
SCREEN 9



Initscreen:
' Open image file and restore
        DEF SEG = VARSEG(img%(1, 1))
        BLOAD "imagesII.dat", VARPTR(img%(1, 1))
        DEF SEG


x1% = 0: x2% = 3: y1% = 0: y2% = 0    'Box start X and start Y
a1% = 0: a2% = 10: b1% = 1: b2% = 5   'Box start X and start Y

x = 160: y = 15: sy = 15
ex = 100: ey = 100

ship = 1
smv = 4
PUT (x, sy), img%(1, 1)
an1 = 9
an2 = 10
shn = 7                ' Enemy ship start at 15
enmycnt = 8
count = 1
        GOSUB initenemies
        
        ON TIMER(1) GOSUB shootatme
        TIMER ON

main:
       
       
       
        GOSUB moveship
        GOSUB moveshots
        GOSUB movenemy
       GOSUB shootatme
        GOTO main


' Subroutine to move your ship across the heavens
moveship:
        a$ = INKEY$: IF a$ = "" THEN GOTO gohere
        IF ship = 1 THEN PUT (x, sy), img%(1, 1), XOR
        IF ship = 2 THEN PUT (x, sy), img%(1, 2), XOR
        IF a$ = "2" THEN sy = sy + 8
        IF a$ = "8" THEN sy = sy - 8
        IF a$ = "4" THEN ship = 2: x = x - 8: smv = -4
        IF a$ = "6" THEN ship = 1: x = x + 8: smv = 4
        IF a$ = "z" THEN GOSUB fireshot
        IF ship = 1 THEN PUT (x, sy), img%(1, 1), XOR
        IF ship = 2 THEN PUT (x, sy), img%(1, 2), XOR
gohere:
        RETURN


'move shot across the screen
moveshots:
        FOR sh = 1 TO 40
        IF sht(1, 1, sh) = 0 THEN GOTO sap
        
        nx = sht(1, 1, sh)
        ny = sht(1, 2, sh)
        IF nx > 590 OR nx < 10 THEN sht(1, 1, sh) = 0: PUT (nx, ny + 2), img%(1, 5), XOR: GOTO sap
       
       
        PUT (nx, ny + 2), img%(1, 5), XOR
       
        nx = nx + sht(1, 3, sh)
        sht(1, 1, sh) = nx
       
        PUT (nx, ny + 2), img%(1, 5)
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
        sht(1, 2, i) = sy: PUT (x, sy + 2), img%(1, 5)
        sht(1, 3, i) = smv
      
       
        RETURN

initenemies:
   
    FOR e = 1 TO enmycnt
   
        RANDOMIZE TIMER
        ex% = INT(RND * 500) + 1
        ey% = INT(RND * 300) + 1
        dir = INT(RND * 3) + 1
        speed = INT(RND * 3) + 3
        enmy(1, e) = ex%
        enmy(2, e) = ey%
        enmy(3, e) = dir        'direction the ship travells
'        enmy(4, e) = speed        'direction the ship travells
      
'        LOCATE 1, 1: PRINT enmy(4, e)
'        FOR t = 1 TO 8000: NEXT t

        PUT (enmy(1, e), enmy(2, e)), img%(1, shn), XOR
        NEXT e
        RETURN


movenemy:
      
       
       FOR j = 1 TO enmycnt
       PUT (enmy(1, j), enmy(2, j)), img%(1, shn), XOR
       
       IF (enmy(3, j)) = 1 THEN enmy(1, j) = enmy(1, j) - 8: PUT (enmy(1, j), enmy(2, j)), img%(1, shn), XOR' if flag = 1 then move left
       IF (enmy(3, j)) = 2 THEN enmy(1, j) = enmy(1, j) + 8: PUT (enmy(1, j), enmy(2, j)), img%(1, shn), XOR' if flag = 2 then move right
       IF (enmy(3, j)) = 3 THEN enmy(1, j) = enmy(1, j) + 2: PUT (enmy(1, j), enmy(2, j)), img%(1, shn), XOR' if flag = 2 then move right
      
       IF enmy(1, j) < 10 AND enmy(3, j) = 1 THEN enmy(3, j) = 2
       IF enmy(1, j) > 500 AND enmy(3, j) = 2 THEN enmy(3, j) = 1
       IF enmy(1, j) > 500 AND enmy(3, j) = 3 THEN enmy(3, j) = 1
      
       NEXT j
      
       LOCATE 1, 1: PRINT shn
       'OR t = 1 TO 6000: NEXT t
       shn = shn + 1                    ' Turn animation ON/OFF
      
       IF shn = 11 THEN shn = 7
       'LOCATE 1, 1: PRINT shn
       'FOR t = 1 TO 8000: NEXT t
       RETURN

shootatme:
        IF enemyshotmoving = 1 THEN GOTO dontrecordit
        SOUND 450, .5
        RANDOMIZE TIMER
        esn = INT(RND * enmycnt) + 1    ' Grab any ship and hold X & Y
        ex = enmy(1, esn)               ' Enemy ship X
        ey = enmy(2, esn)               ' enemy ship Y
        mx = x: my = sy                 ' record were my ship is
                                        ' after 7 seconds shoot at me
                                        ' work toward that goal
        PUT (ex, ey), img%(1, 6), XOR

dontrecordit:
        enemyshotmoving = 1
       
        IF ex < mx THEN ex = ex + 8:
        IF ex > mx THEN ex = ex - 8:
        IF ey > my THEN ey = ey - 8: PUT (ex, ey + 8), img%(1, 6), XOR
        IF ey < my THEN ey = ey + 8: PUT (ex, ey - 8), img%(1, 6), XOR
        tmp1 = ey - my
        tmp2 = ex - mx

        IF tmp1 < 8 AND tmp2 < 8 THEN enemyshotmoving = 0: PRINT "enmy"
        LOCATE 2, 1: PRINT tmp1, tmp2

        PUT (ex, ey), img%(1, 6), XOR
        
        RETURN


       

