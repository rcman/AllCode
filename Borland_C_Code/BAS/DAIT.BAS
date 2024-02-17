   
    'his example requires a color graphics adapter.
    SCREEN 9
    DIM box%(100, 200)
    cl = 1
    FOR i = 1 TO 10
    x1% = 0: x2% = 10: y1% = 0: y2% = 10
    LINE (x1%, y1%)-(x2%, y2%), cl, BF
    GET (x1%, y1%)-(x2%, y2%), box%(1, i)
    cl = cl + 1
    NEXT i
    sx = 30: sy = 10
    FOR j = 1 TO 10
    PUT (sx, sy), box%(1, j)
    sx = sx + 30
    NEXT j

