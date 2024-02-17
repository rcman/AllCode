
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
        BLOAD "cars.dat", VARPTR(img%(1, 1))
        DEF SEG


x1% = 0: x2% = 3: y1% = 0: y2% = 0    'Box start X and start Y
a1% = 0: a2% = 10: b1% = 1: b2% = 5   'Box start X and start Y

rdspd = 2: count = 0

x = 260: y = 215: sy = 215
ex = 100: ey = 100

rd1 = 260: rd2 = 1
car1 = 260: car2 = 60: carside = 0
PUT (car1, car2), img%(1, 15), XOR

cop1 = 260: cop2 = 300: trueval = 13: counter = 0: cx = 260
'PUT (cop1, cop2), img%(1, 13), XOR

pot1 = 276: pot2 = 1:
PUT (pot1, pot2), img%(1, 4), XOR


ship = 1
smv = 4
PUT (x, y), img%(1, 1)
'PUT (rd1, rd2), img%(1, 10)

LINE (160, 1)-(160, 315), 1
LINE (360, 1)-(360, 315), 1

carloc = 0

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



        'ON TIMER(2) GOSUB drawcopter
        'TIMER ON


         GOSUB DrawRoad
main:
        counter = counter + 1
        GOSUB moveship
       GOSUB moveroad
        GOSUB drawpothole
        GOSUB drawothercar
        GOSUB drawcopter
       'GOSUB mose


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
        IF pot2 > 300 THEN pot2 = 1
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

drawothercar:
        
        PUT (car1, car2), img%(1, 15), XOR
        car2 = car2 + rdspd
        RANDOMIZE TIMER: carloc = INT(RND * 40) + 1:
        IF car2 > 290 THEN car2 = 1:  IF carside = 0 THEN car1 = car1 - carloc: carside = 1: GOTO h1
        IF carside = 1 THEN car1 = car1 + carloc: carside = 0
h1:   
        PUT (car1, car2), img%(1, 15), XOR
        RETURN

drawcopter:
        IF counter < 50 THEN RETURN
        SOUND 40, .2:
        PUT (cx, cop2), img%(1, trueval), XOR
        cop2 = cop2 - rdspd - 4
        IF cop2 < 15 THEN cop2 = 300: counter = 0: cx = x
        'IF cop2 < 15 THEN cop2 = 300: PUT (cop1, cop2), img%(1, trueval), XOR: counter = 0
        trueval = trueval + 1: IF trueval > 14 THEN trueval = 13
        PUT (cx, cop2), img%(1, trueval), XOR
       
        RETURN

