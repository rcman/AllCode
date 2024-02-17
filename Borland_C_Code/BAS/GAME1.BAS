' Shooting Routine

'DIM Sht(x,y,n)
DIM Sht(10, 10, 80)

DIM Box%(1 TO 200)
SCREEN 1
x1% = 0: x2% = 3: y1% = 0: y2% = 0    'Box start X and start Y
'LINE (x1%, y1%)-(x2%, y2%), 2, BF       'Draw Box with Box Fill
LINE (x1%, y1%)-(x2%, y2%), 2        'Draw Box with Box Fill
GET (x1%, y1%)-(x2%, y2%), Box%         ' get the shoot

shoot:
 


    DO
        PUT (x1%, y1%), Box%, XOR
        x1% = RND * 300
        y1% = RND * 180
        PUT (x1%, y1%), Box%
    LOOP WHILE INKEY$ = ""
END












Example:
    'This example requires a color graphics adapter.
    

  
    GET (x1%, y1%)-(x2%, y2%), Box%
    DO
        PUT (x1%, y1%), Box%, XOR
        x1% = RND * 300
        y1% = RND * 180
        PUT (x1%, y1%), Box%
    LOOP WHILE INKEY$ = ""
END

