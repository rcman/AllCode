start:
        a = INP(&H201)
        IF (a AND 32) = 0 THEN PRINT "button 1"
        IF (a AND 16) = 0 THEN PRINT "button 2"
        GOTO start

