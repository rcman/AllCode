' Parser

source$ = "c:\myfile.c"
destin$ = "c:\destin.bas"
tables$ = "c:\mytable.tbl"
DIM s$(1000)
DIM sname$(1000), dname$(1000)
count = 1
a = 1

start:
       
    '    PRINT "Reading Source Table"
        OPEN tables$ FOR INPUT AS #1
DO WHILE NOT EOF(1)
        INPUT #1, sname$(a), dname$(a)
        PRINT sname$(a), dname$(a)
        a = a + 1

LOOP
CLOSE #1

     '   PRINT "Reading Source File"
        OPEN source$ FOR INPUT AS #1
DO WHILE NOT EOF(1)
        LINE INPUT #1, s$(a)
        count = count + 1
        a = a + 1
LOOP
CLOSE #1

      '  PRINT "Opening Destination File"
        OPEN destin$ FOR OUTPUT AS #1


Parse:
        b = 1
        ' PRINT count

nxt:
        FOR j = 1 TO count + 1
       ' PRINT "Current Line"; s$(b)
jmp:
        
       ' xl = LEN(s$(b))
       ' PRINT s$(b)

'       IF xl < 1 THEN b = b + 1: GOTO nxt

       TMP$ = RTRIM$(s$(b))
       TMP$ = LTRIM$(s$(b))
       
        xl = LEN(TMP$)
       ' PRINT "the length of line is "; xl

        FOR i = 1 TO xl
        MY$ = MID$(TMP$, i, 1)          ' 1 Character
        twl$ = MID$(TMP$, i, 2)         ' First 2 Letters
        com$ = MID$(TMP$, i, 5)         ' First 5 Letters

        IF twl$ = "#i" THEN GOTO inc
        IF twl$ = "#d" THEN GOTO df
        IF com$ = "print" THEN GOTO prt
back:

        NEXT i
        b = b + 1
        NEXT j
'       PRINT count
        CLOSE 1
        END

inc:
' Found #include
        PRINT d$(b)
        GOTO back

df:
' Found #define
        PRINT "define"
        GOTO back

prt:
' found a printf
        PRINT "heekldsjfhskl"
        PRINT b
        PRINT dname$(b)
        GOTO back

