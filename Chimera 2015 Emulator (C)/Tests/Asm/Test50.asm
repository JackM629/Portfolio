LDZ #0x342A
LDAA #0x00E3
TAP  
LDAA #0x00BB
STA 0xAADD
LDAA #0x00BB
STA 0xAADE
LD C,#0x0023
LDAA #0x0089
EOR A,C
STA 0x01FA
LD C,#0x00B8
LDAA #0x0042
EOR A,C
STA 0x01FB
LD C,#0x00C9
LDAA #0x0074
EOR A,C
STA 0x01FC
LD C,#0x0098
LDAA #0x001C
EOR A,C
STA 0x01FD
LD C,#0x00F2
LDAA #0x00EE
EOR A,C
STA 0x01FE
LD C,#0x004D
LDAA #0x00C6
EOR A,C
STA 0x01FF
LD C,#0x00D6
LDAA #0x00EF
EOR A,C
TSA
STA 0x0200
LD C,#0x0019
LDAA #0x007C
EOR A,C
TSA
STA 0x0201
LD C,#0x0017
LDAA #0x0063
EOR A,C
TSA
STA 0x0202
LD C,#0x0097
LDAA #0x00EA
EOR A,C
TSA
STA 0x0203
LD C,#0x00A3
LDAA #0x00F6
EOR A,C
TSA
STA 0x0204
LD C,#0x00D7
LDAA #0x00E3
EOR A,C
TSA
STA 0x0205
HALT 
