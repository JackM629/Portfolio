LDZ #0x342A
LDAA #0x00E3
TAP  
LDAA #0x00BB
STA 0xAADD
LDAA #0x00BB
STA 0xAADE
LD C,#0x00E7
LDAA #0x003E
ORA A,C
STA 0x01FA
LD C,#0x001D
LDAA #0x00D7
ORA A,C
STA 0x01FB
LD C,#0x002B
LDAA #0x0027
ORA A,C
STA 0x01FC
LD C,#0x00F6
LDAA #0x0083
ORA A,C
STA 0x01FD
LD C,#0x0043
LDAA #0x00D1
ORA A,C
STA 0x01FE
LD C,#0x0009
LDAA #0x005A
ORA A,C
STA 0x01FF
LD C,#0x00EA
LDAA #0x001B
ORA A,C
TSA
STA 0x0200
LD C,#0x0079
LDAA #0x0063
ORA A,C
TSA
STA 0x0201
LD C,#0x0004
LDAA #0x00E3
ORA A,C
TSA
STA 0x0202
LD C,#0x00F7
LDAA #0x0029
ORA A,C
TSA
STA 0x0203
LD C,#0x00C5
LDAA #0x008B
ORA A,C
TSA
STA 0x0204
LD C,#0x0049
LDAA #0x0051
ORA A,C
TSA
STA 0x0205
HALT 
