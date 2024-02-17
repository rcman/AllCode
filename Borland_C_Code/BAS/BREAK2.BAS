SCREEN 9
CL = 1
DIM stars(2, 60)
DIM bx%(140, 200)
DIM box2%(1 TO 200)
DIM crl%(1 TO 400)
DIM hit(6, 12)
DIM m%(30)

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

lp = 0: CL = 1: mx = 30: my = 30:
ax = 200: ay = 200: DR = 1
xo = 1: yo = -1
        ox = ax
        oy = ay

    CIRCLE (10, 10), 5, 11
    PAINT (11, 11), 11, 11
    GET (5, 5)-(15, 15), crl%
       
    x1% = 0: x2% = 40: y1% = 0: y2% = 10
    LINE (x1%, y1%)-(x2%, y2%), 11, BF
    GET (x1%, y1%)-(x2%, y2%), box2%
'    PUT (x1%, y1%), Box2%
   
    

       
hm:
    x1% = 0: x2% = 40: y1% = 0: y2% = 10
    FOR i = 0 TO 16
    CLS
    lp = lp + 1
    LINE (x1%, y1%)-(x2%, y2%), CL, BF
    GET (x1%, y1%)-(x2%, y2%), bx%(1, i)
    CL = CL + 1
    NEXT i

        CLS
    num = 40: x = 100: y = 330: d = 1: savx = x
        DEF SEG = VARSEG(m%(0))
        CALL ABSOLUTE(VARPTR(m%(0)))
        DEF SEG
        savx = INT(m%(18) / 1.1)
        x = savx

    PUT (x, y), box2%
    PUT (ox, oy), crl%
        cr = 1
        CL = 9
       
       'GOSUB initstars
        GOSUB drawlevel
        GOSUB askme
       
mvstrs:
       
        GOSUB gtkey
        GOSUB putbox
        GOSUB mvball

        GOTO mvstrs

edpgm:
        a$ = INKEY$: IF a$ = "" THEN GOTO edpgm
        END


initstars:
      RANDOMIZE TIMER
      TIMER ON
      FOR i = 1 TO 40
       x1% = INT(RND * 640) + 1
       y1% = INT(RND * 350) + 1
          stars(1, i) = x1%
          stars(2, i) = y1%
          NEXT i
          RETURN

drawstars:
        CL = 1
       
        FOR i = 1 TO 39
        PSET (stars(1, i), stars(2, i)), CL
        CL = CL + 1
        IF CL > 14 THEN CL = 1
        NEXT i
        RETURN


drawstars2:
        

        FOR i = 1 TO 39
        PSET (stars(1, i), stars(2, i)), CL
        IF CL > 14 THEN CL = 1
        CL = CL + 1
        'FOR t = 1 TO 600: NEXT
        NEXT i
        RETURN

putbox:
        IF savx = x THEN RETURN
        PUT (x, y), box2%, XOR
        PUT (savx, y), box2%, XOR
        savx = x
        RETURN

gtkey:
        DEF SEG = VARSEG(m%(0))
        CALL ABSOLUTE(VARPTR(m%(0)))
        DEF SEG
        x = INT(m%(18) / 1.1)
        'LOCATE 1, 1: PRINT x
        'my = m%(17) / 9
        'mb = m%(19)
        b$ = INKEY$: IF b$ = "" OR savx = x THEN nomove = 1: GOTO cont1
        IF savx <> x THEN b$ = "4": GOTO cont1
        'savx = x
        IF b$ = "4" THEN x = x - 16: IF x < 10 THEN x = x + 16
        IF b$ = "6" THEN x = x + 16: IF x > 580 THEN x = x - 16
cont1:
        RETURN


drawlevel:
        col = 10: row = 60: numrow = 0: obj = 1

drawit:
        
        FOR T = 1 TO 30: NEXT T
        PUT (col, row), bx%(1, obj), XOR
'        hit(1, hy) = col
'        hit(2, hy) = row
'        hit(3, n) = obj
        SOUND 480, .1
        'FOR T = 1 TO 130: NEXT T
        col = col + 47
        obj = obj + 1
        IF obj >= 15 THEN obj = 1
        IF col >= 580 THEN col = 10: row = row + 13: numrow = numrow + 1
        IF numrow = 6 THEN GOTO back
        GOTO drawit

back:
        hitval = obj
        RETURN

crcl:
        

    'LINE (10, 10)-(1, 1), 11, BF
    RETURN

waithere:

        z$ = INKEY$: IF z$ = "" THEN GOTO waithere
        RETURN

hitval:
      ' FOR i = 1 TO hitval
        temp1 = hit(1, i)
        temp2 = hit(2, i)
       
        'hit(1, hy) = col
        'hit(2, hy) = row
       'if hit(3, n) = obj

mvball:

        IF ax < 10 OR ax > 600 THEN
                xo = -xo
        END IF
        IF ay < 10 OR ay > 338 THEN
                yo = -yo
        END IF
        'LOCATE oy, oxe: PRINT " ";
        PUT (ax, ay), crl%, XOR
        PUT (ox, oy), crl%, XOR
        IF POINT(ax, ay) > 0 OR POINT(ax, ay + 7) > 0 THEN
                IF ay < 320 THEN GOSUB hitblock
                yo = -yo
        END IF
        ox = ax
        oy = ay
        ay = ay + yo
        ax = ax + xo
        'FOR i = 1 TO 100: NEXT i
        RETURN

        'FOR T = 1 TO 100: NEXT T
        'LOCATE 1, 1: PRINT ax, ay

HT:
        PUT (ax, ay), crl%
        RETURN

askme:

        LOCATE 15, 35: PRINT "GET READY!"
        GOSUB waithere
        LOCATE 15, 35: PRINT "          "

        RETURN


hitblock:
        hax = INT((ax - 10) / 47)
        hay = INT((ay - 60) / 13)
        hcl = POINT(ax, ay)
        LOCATE 1, 1: PRINT hax, hay
        PUT (hax * 47 + 10, hay * 13 + 60), bx%(1, hcl)
        RETURN

