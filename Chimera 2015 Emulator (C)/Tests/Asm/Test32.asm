LDZ #0x342A
LDAB #0x00E3
TAP  
LDAB #0x00BB
STB 0xAADD
LDAB #0x00BB
STB 0xAADE
LD C,#0x0006
LDAB #0x0026
ADD B,C
STB 0x01FA
LD C,#0x008C
LDAB #0x00C0
ADD B,C
STB 0x01FB
LD C,#0x0027
LDAB #0x00A9
ADD B,C
STB 0x01FC
LD C,#0x0090
LDAB #0x0098
ADD B,C
STB 0x01FD
LD C,#0x00AF
LDAB #0x0089
ADD B,C
STB 0x01FE
LD C,#0x00C5
LDAB #0x004F
ADD B,C
STB 0x01FF
LD C,#0x004B
LDAB #0x00C5
ADD B,C
TSA
STA 0x0200
LD C,#0x006D
LDAB #0x0024
ADD B,C
TSA
STA 0x0201
LD C,#0x006B
LDAB #0x0002
ADD B,C
TSA
STA 0x0202
LD C,#0x003F
LDAB #0x0011
ADD B,C
TSA
STA 0x0203
LD C,#0x006A
LDAB #0x0047
ADD B,C
TSA
STA 0x0204
LD C,#0x009D
LDAB #0x0079
ADD B,C
TSA
STA 0x0205
HALT 
