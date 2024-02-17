' Game like data storm on Amiga

CLEAR , , 80000

DIM img%(1 TO 1000, 1 TO 32)            ' Images stored in .DAT file
DIM sht(10, 10, 80)
DIM enmy(1, 2, 20)
DIM Box%(1 TO 200)
DIM shp1%(200)
DIM shp2%(200)
SCREEN 9



Initscreen:
' Open image file and restore
        DEF SEG = VARSEG(img%(1, 1))
        BLOAD "images2.dat", VARPTR(img%(1, 1))
        DEF SEG


x1% = 0: x2% = 3: y1% = 0: y2% = 0    'Box start X and start Y
a1% = 0: a2% = 10: b1% = 1: b2% = 5   'Box start X and start Y

x = 160: y = 15: sy = 15
ship = 1
smv = 4
PUT (x, sy), img%(1, 1)
an1 = 9
an2 = 11

        GOSUB initenemies


main:
       ' FOR i = 1 TO 50: NEXT i

        GOSUB moveship
        GOSUB moveshots
        GOSUB movenemy
        GOSUB checkhit
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
   
    FOR e = 1 TO 20
       
        ex% = INT(RND * 500) + 1
        ey% = INT(RND * 300) + 1
        enmy(1, 1, e) = ex%
        enmy(1, 2, e) = ey%
        NEXT e
        RETURN


movenemy:
      
       
       FOR j = 1 TO 20

       IF enmy(1, 1, j) = 0 THEN GOTO shht
       IF enmy(1, 2, j) = 0 THEN GOTO shht
            
       'PUT (enmy(1, 1, j), enmy(1, 2, j)), img%(1, an1), XOR
       PUT (enmy(1, 1, j), enmy(1, 2, j)), img%(1, an1), XOR
       IF enmy(1, 1, j) > x THEN enmy(1, 1, j) = enmy(1, 1, j) - 3
       IF enmy(1, 2, j) > sy THEN enmy(1, 2, j) = enmy(1, 2, j) - 3
       IF enmy(1, 1, j) < x THEN enmy(1, 1, j) = enmy(1, 1, j) + 3
       IF enmy(1, 2, j) < sy THEN enmy(1, 2, j) = enmy(1, 2, j) + 3
       'PUT (enmy(1, 1, j), enmy(1, 2, j)), img%(1, an1), XOR
       PUT (enmy(1, 1, j), enmy(1, 2, j)), img%(1, an1), XOR
shht: 
       NEXT j
     an1 = an1 + 1
      IF an1 > 10 AND enmy(1, 1, j) = 0 OR an1 > 10 AND enmy(1, 2, j) = 0 THEN an1 = an1 + 1
      IF an1 > 10 AND enmy(1, 1, j) = 0 OR an1 > 10 AND enmy(1, 2, j) = 0 AND an1 = 13 THEN GOTO skipanim

      IF an1 > 10 AND enmy(1, 1, j) > 0 THEN an1 = 9
      

      ' FOR t = 1 TO 4000: NEXT t
      'FOR t = 1 TO 4000: NEXT t
      'LOCATE 4, 1: PRINT an1

skipanim:

        RETURN
checkhit:
        FOR j = 1 TO 20
      
'       IF enmy(1, 1, j) = 0 THEN GOTO explode
'       IF enmy(1, 2, j) = 0 THEN GOTO explode
        
        IF enmy(1, 1, j) = x THEN enmy(1, 1, j) = 0
        IF enmy(1, 2, j) = sy THEN enmy(1, 2, j) = 0
        IF enmy(1, 1, j) = x THEN enmy(1, 1, j) = 0
        IF enmy(1, 2, j) = sy THEN enmy(1, 2, j) = 0
        NEXT j
        RETURN

'explode:


