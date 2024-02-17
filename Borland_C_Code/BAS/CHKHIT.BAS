
SCREEN 9
CL = 1
DIM stars(2, 60)
DIM bx%(140, 200)
DIM hit(3, 128)
DIM chk(128)


DEF SEG = VARSEG(bx%)

    DIM crl%(1 TO 400)
    CIRCLE (10, 10), 5, 11
    GET (5, 5)-(15, 15), crl%
    PUT (200, 200), crl%
    'GOSUB waithere
    'END

    DIM Box2%(1 TO 200)
    x1% = 0: x2% = 40: y1% = 0: y2% = 10
    LINE (x1%, y1%)-(x2%, y2%), 11, BF
    GET (x1%, y1%)-(x2%, y2%), Box2%
    PUT (x1%, y1%), Box2%
   
    lp = 0: CL = 0: tmp1 = 0: tmp2 = 0: tmp3 = 0: tmp4 = 0
    x1% = 0: x2% = 40: y1% = 0: y2% = 10: ax = 200: ay = 140: dr = 1
    mx = 30: my = 30
       
hm:

    FOR i = 0 TO 16
    lp = lp + 1
    LINE (x1%, y1%)-(x2%, y2%), CL, BF
    GET (x1%, y1%)-(x2%, y2%), bx%(1, i)
    CL = CL + 1
    NEXT i

    num = 40: x = 100: y = 330: d = 1
    PUT (x, y), Box2%
        cr = 1
        CL = 9
       
        GOSUB drawlevel
        GOSUB crcl
mvstrs:
        GOSUB gtkey
        GOSUB putbox
        GOSUB mvball
        GOSUB chkhit
        GOTO mvstrs
edpgm:
        a$ = INKEY$: IF a$ = "" THEN GOTO edpgm

        END

putbox:
        IF nomove = 1 THEN GOTO ov
        IF x < 10 THEN x = x + 8: GOTO ov
        IF x > 580 THEN x = x - 8: GOTO ov
        IF b$ = "4" THEN PUT (x + 8, y), Box2%, XOR: PUT (x, y), Box2%, XOR:
        IF b$ = "6" THEN PUT (x - 8, y), Box2%, XOR: PUT (x, y), Box2%, XOR
ov:
        RETURN

gtkey:
        b$ = INKEY$: IF b$ = "" THEN nomove = 1: GOTO cont1
        nomove = 0
        IF b$ = "4" THEN x = x - 8
        IF b$ = "6" THEN x = x + 8
cont1:
        RETURN


drawlevel:
        col = 10: row = 10: numrow = 0: obj = 1: nm = 1


drawit:
        
        PUT (col, row), bx%(1, obj), XOR
        hit(1, nm) = col
        hit(2, nm) = row
        hit(3, nm) = obj
        nm = nm + 1
        'SOUND 480, .1
        FOR t = 1 TO 130: NEXT t
        col = col + 44
        obj = obj + 1
        
        IF obj >= 15 THEN obj = 1
        IF col >= 610 THEN col = 10: row = row + 13: numrow = numrow + 1
        IF numrow = 6 THEN GOTO back
        'FOR t = 1 TO 2000: NEXT t
        'PUT (200, 300), bx%(10), XOR
        GOTO drawit

back:
        RETURN

test:

        
        RETURN
crcl:
        PUT (100, 300), crl%

    'LINE (10, 10)-(1, 1), 11, BF
    RETURN

waithere:

        z$ = INKEY$: IF z$ = "" THEN GOTO waithere
        RETURN

mvball:
        IF ay < 10 THEN dr = 1
        IF ay > 300 THEN dr = 2
        IF dr = 1 THEN ay = ay + 5: PUT (ax, ay), crl%, XOR: PUT (ax, ay - 5), crl%, XOR
        IF dr = 2 THEN ay = ay - 5: PUT (ax, ay), crl%, XOR: PUT (ax, ay + 5), crl%, XOR
        'FOR t = 1 TO 150: NEXT t
       
        RETURN

chkhit:
        GOSUB chkzero

        FOR u = 1 TO nm - 1
        tmp1 = hit(1, u)
        tmp2 = hit(2, u)
        tmpc = hit(3, u)
        IF tmp1 = 0 THEN LOCATE 7, 1: PRINT hit(1, u): GOTO ov4
        tmp3 = ax - tmp1
        tmp4 = ay - tmp2
        IF tmp3 < 2 AND tmp4 < 12 THEN dr = 1: GOSUB pb

ov4:
        NEXT u
        cnm = 0

        RETURN


pb:
        IF cnm = 1 THEN : RETURN
        'LOCATE 3, 1: PRINT tmp1, tmp2
        'GOSUB waithere
        PUT (tmp1 - 44, tmp2), bx%(1, tmpc - 1)
'        PRINT u, hit(1, u), hit(2, u)
        GOSUB waithere
        hit(1, u - 1) = 0
        hit(2, u - 1) = 0
        tmp3 = 99
        tmp4 = 99
        LOCATE 6, 5: PRINT u - 1:
        cnm = 1
        RETURN
chkzero:
        FOR i = 1 TO nm - 1
        IF hit(1, i) = 0 OR hit(2, i) = 0 THEN PRINT "sdhgjskadhflkaj"
        NEXT i
        RETURN



