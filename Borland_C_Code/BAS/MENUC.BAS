
DIM a$(6)
numelm = 5
xc = 0
a$(0) = "Title"
a$(1) = "Accounting this ios c centred"
a$(2) = "his ios c centred"
a$(3) = "Accountid"
a$(4) = "dfjldfkjglskdjhklAccountin ios c centred"
a$(5) = "Accountinntred "

' determine maximum x distance of box

FOR i = 0 TO 5
        xl = LEN(a$(i))
        IF xl > xc THEN xc = xl
        NEXT i
        LOCATE 21, 1: PRINT "Maximum Length is "; xc
       

x = LEN(a$(1))
PRINT x
y = (80 - x) / 2
PRINT y
LOCATE 1, y: PRINT a$(1)

