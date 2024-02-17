' Parser

table$ = "c:\mytable.tbl"

f1$ = "print"
f2$ = "printf"

start:
        PRINT "Saving Source Table"
        OPEN table$ FOR OUTPUT AS 1
        PRINT #1, f1$, f2$
        CLOSE #1

