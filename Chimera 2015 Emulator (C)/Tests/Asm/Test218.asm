LDZ #0x342A
LDAB #0x00E3
TAP  
LDAB #0x00BB
STB 0xAADD
LDAB #0x00BB
STB 0xAADE
LDAB #0x0062
LD C,#0x00C8
LD D,#0x00CB
CMP A,CSTB 0x01FA
LDAB #0x0039
LD C,#0x00A5
LD D,#0x0065
CMP A,CSTB 0x01FB
LDAB #0x002F
LD C,#0x0032
LD D,#0x0005
CMP A,CSTB 0x01FC
LDAB #0x00F4
LD C,#0x0046
LD D,#0x00B1
CMP A,CSTB 0x01FD
LDAB #0x00D9
LD C,#0x003A
LD D,#0x00F1
CMP A,CSTB 0x01FE
LDAB #0x00CB
LD C,#0x00F4
LD D,#0x0077
CMP A,CSTB 0x01FF
LDAB #0x00F7
LD C,#0x0081
LD D,#0x0055
CMP A,CTSA
STA 0x0200
LDAB #0x002C
LD C,#0x004A
LD D,#0x005B
CMP A,CTSA
STA 0x0201
LDAB #0x005D
LD C,#0x0021
LD D,#0x0062
CMP A,CTSA
STA 0x0202
LDAB #0x00F9
LD C,#0x0053
LD D,#0x00CF
CMP A,CTSA
STA 0x0203
LDAB #0x00AC
LD C,#0x0018
LD D,#0x0022
CMP A,CTSA
STA 0x0204
LDAB #0x00DB
LD C,#0x006A
LD D,#0x0047
CMP A,CTSA
STA 0x0205
HALT 