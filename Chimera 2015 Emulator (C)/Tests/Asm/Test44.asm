LDZ #0x342A
LDAB #0x00E3
TAP  
LDAB #0x00BB
STB 0xAADD
LDAB #0x00BB
STB 0xAADE
LD C,#0x0095
LDAB #0x0042
ORA B,C
STB 0x01FA
LD C,#0x00DC
LDAB #0x00F7
ORA B,C
STB 0x01FB
LD C,#0x005E
LDAB #0x00EA
ORA B,C
STB 0x01FC
LD C,#0x001A
LDAB #0x00EC
ORA B,C
STB 0x01FD
LD C,#0x004C
LDAB #0x0028
ORA B,C
STB 0x01FE
LD C,#0x0075
LDAB #0x000C
ORA B,C
STB 0x01FF
LD C,#0x0021
LDAB #0x000B
ORA B,C
TSA
STA 0x0200
LD C,#0x0058
LDAB #0x0065
ORA B,C
TSA
STA 0x0201
LD C,#0x009A
LDAB #0x0069
ORA B,C
TSA
STA 0x0202
LD C,#0x0010
LDAB #0x00D1
ORA B,C
TSA
STA 0x0203
LD C,#0x0043
LDAB #0x0029
ORA B,C
TSA
STA 0x0204
LD C,#0x0013
LDAB #0x0054
ORA B,C
TSA
STA 0x0205
HALT 
