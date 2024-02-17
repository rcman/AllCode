DIM enimies(50, 2)         ' 50 enimie ships and thier x & y coordinates
DIM ys(30, 2)              ' 30 shoots and the x & y coordinates
DIM es(50, 2)              ' 50 shoots for the enimies and x & y

x = 20          ' your ships X
y = 100         ' your ships Y

SCREEN 9        ' 640 X 350  in 16 colours

mainloop:
        GOSUB moveship
        GOSUB moveshots
        GOTO mainloop

' Subroutine to move your ship across the heavens
moveship:
        a$ = INKEY$
        IF a$ = "2" THEN y = y + 1
        IF a$ = "8" THEN y = y - 1
        IF a$ = " " THEN GOSUB fireshot
        RETURN

' Subroutine to move shoots across screen for your ship
' the starting position will be your ships x & y for each shot
moveshots:
        FOR i = 30 TO 1 STEP -1
                ' the line below checks if there is a shot and if so blank shot
                IF ys(i - 1, 1) < 9 OR ys(i - 1, 1) > 630 THEN ys(i - 1, 1) = 0: ys(i - 1, 2) = 0: GOTO noshot
                
                LINE (ys(i, 1), ys(i, 2))-(ys(i, 1) + 5, ys(i, 2)), 0
                ys(i, 1) = ys(i - 1, 1)
                ys(i, 2) = ys(i - 1, 2)
                ys(i, 1) = ys(i, 1) + 4
                LINE (ys(i, 1), ys(i, 2))-(ys(i, 1) + 5, ys(i, 2)), 3
noshot:
        NEXT i
        RETURN

' Subroutine to fire shot from your ship
fireshot:
        ys(1, 1) = x
        ys(1, 2) = y
        LOCATE 2, 1: PRINT ys(1, 1), ys(1, 2)
        RETURN


