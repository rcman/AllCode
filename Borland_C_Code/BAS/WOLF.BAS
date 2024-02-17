
DIM img%(1 TO 500, 1 TO 32)            ' Images stored in .DAT file
DIM sht(10, 10, 80)

DIM enmy(4, 30)                         ' Enemy Array
DIM savenmy(4, 30)                         ' Enemy Array
DIM m%(30)
DIM Box%(1 TO 200)
DIM shp1%(200)
DIM shp2%(200)
SCREEN 9



Initscreen:
' Open image file and restore
        DEF SEG = VARSEG(img%(1, 1))
        BLOAD "wolf.dat", VARPTR(img%(1, 1))
        DEF SEG


x1% = 0: x2% = 3: y1% = 0: y2% = 0    'Box start X and start Y
a1% = 0: a2% = 10: b1% = 1: b2% = 5   'Box start X and start Y

rdspd = 2: count = 0

x = 260: y = 215: sy = 215
ex = 100: ey = 100

rd1 = 260: rd2 = 1
pot1 = 276: pot2 = 280:

ship = 1
smv = 4
PUT (x, y), img%(1, 1)
'PUT (rd1, rd2), img%(1, 10)

'LINE (160, 1)-(160, 315), 1
'LINE (360, 1)-(360, 315), 1

an1 = 9
an2 = 10
shn = 7                ' Enemy ship start at 15
savshn = 7
enmycnt = 8
count = 1


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



        'ON TIMER(2) GOSUB drawpothole
        'TIMER ON


 '        GOSUB DrawRoad
main:
       
     
         'GOSUB moveship
      ' GOSUB moveroad
      '  GOSUB drawpothole
      ' GOSUB mose
       GOSUB animcar
        'GOSUB wave
        GOSUB walk


    '    GOSUB movenemy
    '    GOSUB shootatme
        GOTO main


' Subroutine to move your ship across the heavens
moveship:
        a$ = INKEY$: IF a$ = "" THEN GOTO gohere
        IF ship = 1 THEN PUT (x, sy), img%(1, 1), XOR
        IF ship = 2 THEN PUT (x, sy), img%(1, 2), XOR
        IF a$ = "2" THEN sy = sy + 8: rdspd = rdspd - 1: IF rdspd < 6 THEN rdspd = 2
        IF a$ = "8" THEN sy = sy - 8: rdspd = rdspd + 1: IF rdspd > 15 THEN rdspd = 10
        IF a$ = "4" THEN ship = 2: x = x - 8: smv = -4: IF x < 165 THEN x = 165
        IF a$ = "6" THEN ship = 1: x = x + 8: smv = 4: IF x > 325 THEN x = 325
        IF a$ = "b" THEN GOSUB honk
        IF ship = 1 THEN PUT (x, sy), img%(1, 1), XOR
        IF ship = 2 THEN PUT (x, sy), img%(1, 2), XOR
gohere:
        RETURN

DrawRoad:
        count = count + 1
        'LOCATE 1, 1: PRINT rd2: FOR t = 1 TO 4000: NEXT t
        enmy(1, count) = 260
        enmy(2, count) = rd2
        rd2 = rd2 + 40
        PUT (enmy(1, count), enmy(2, count)), img%(1, 6), XOR
        IF rd2 < 320 THEN GOTO DrawRoad
        howmany = count
        RETURN


moveroad:
        FOR i = 2 TO count
        PUT (enmy(1, i), enmy(2, i)), img%(1, 6), XOR
        enmy(2, i) = enmy(2, i) + rdspd
        IF enmy(2, i) > 300 THEN enmy(2, i) = 1
        PUT (enmy(1, i), enmy(2, i)), img%(1, 6), XOR
        NEXT i
        RETURN



drawpothole:
       
        PUT (pot1, pot2), img%(1, 4), XOR
        pot2 = pot2 + rdspd
        IF pot2 > 290 THEN pot2 = 1
        PUT (pot1, pot2), img%(1, 4), XOR
        RETURN

honk:
        SOUND 70, 3
        RETURN

mose:
        DEF SEG = VARSEG(m%(0))
        CALL ABSOLUTE(VARPTR(m%(0)))
        DEF SEG
        x = INT(m%(18) / 13)
        sy = INT(m%(17) / 9)
        LOCATE 1, 1: PRINT x, sy

        RETURN

animcar:



        FOR j = 6 TO 8
        PUT (200, 100), img%(1, j), PSET
        FOR t = 1 TO 2420: NEXT t
        NEXT j
                                                'Head Turning  
        FOR j = 8 TO 6 STEP -1
        PUT (200, 100), img%(1, j), PSET
        FOR t = 1 TO 2420: NEXT t
        NEXT j


        FOR y = 20 TO 22
        PUT (100, 100), img%(1, y), PSET
        FOR t = 1 TO 1420: NEXT t
        NEXT y
                                             'Sleeping
        FOR y = 22 TO 20 STEP -1
        PUT (100, 100), img%(1, y), PSET
        FOR t = 1 TO 1420: NEXT t
        NEXT y

        FOR y = 12 TO 14
        PUT (250, 250), img%(1, y), PSET
        FOR t = 1 TO 1420: NEXT t
        NEXT y
                                             'Blinking
        FOR y = 13 TO 14 STEP -1
        PUT (250, 250), img%(1, y), PSET
        FOR t = 1 TO 1420: NEXT t
        NEXT y

        
        RETURN

wave:
        FOR y = 15 TO 16
        PUT (50, 50), img%(1, y), PSET
        FOR t = 1 TO 2120: NEXT t
        NEXT y
        RETURN

walk:    
        FOR i = 9 TO 11
        PUT (200, 200), img%(1, i), PSET
        FOR t = 1 TO 1420: NEXT t
        NEXT i
                                                'Walking
        FOR i = 11 TO 9 STEP -1
        PUT (200, 200), img%(1, i), PSET
        FOR t = 1 TO 1420: NEXT t
        NEXT i
        RETURN

