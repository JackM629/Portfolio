LDZ #0x342A
LDAA #0x00E3
TAP  
LDAA #0x00BB
STA 0xAADD
LDAA #0x00BB
STA 0xAADE
LD C,#0x0097
LDAA #0x00BE
BT B,C
STA 0x01FA
LD C,#0x002E
LDAA #0x00DE
BT B,C
STA 0x01FB
LD C,#0x00B8
LDAA #0x0019
BT B,C
STA 0x01FC
LD C,#0x0094
LDAA #0x0003
BT B,C
STA 0x01FD
LD C,#0x0069
LDAA #0x0026
BT B,C
STA 0x01FE
LD C,#0x009A
LDAA #0x00D1
BT B,C
STA 0x01FF
LD C,#0x00AE
LDAA #0x0000
BT B,C
TSA
STA 0x0200
LD C,#0x00F3
LDAA #0x000F
BT B,C
TSA
STA 0x0201
LD C,#0x0019
LDAA #0x0014
BT B,C
TSA
STA 0x0202
LD C,#0x00A2
LDAA #0x0040
BT B,C
TSA
STA 0x0203
LD C,#0x007B
LDAA #0x008A
BT B,C
TSA
STA 0x0204
LD C,#0x0017
LDAA #0x0016
BT B,C
TSA
STA 0x0205
HALT 