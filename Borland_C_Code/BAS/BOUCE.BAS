
CLS
axe = 5
ay = 11
oxe = axe
oy = ay
yo = -1
xo = -1


start:                 
        LOCATE ay, axe
        PRINT "O";

        IF axe < 2 OR axe > 79 THEN
                xo = -xo
        END IF
        IF ay < 2 OR ay > 24 THEN
                yo = -yo
        END IF
        LOCATE oy, oxe: PRINT " ";
        oxe = axe
        oy = ay
        ay = ay + yo
        axe = axe + xo
        FOR i = 1 TO 100: NEXT i
        GOTO start


