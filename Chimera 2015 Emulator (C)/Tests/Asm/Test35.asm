LDZ #0x342A
LDAA #0x00E3
TAP  
LDAA #0x00BB
STA 0xAADD
LDAA #0x00BB
STA 0xAADE
LD D,#0x0009
LDAA #0x00F1
SUB A,D
STA 0x01FA
LD D,#0x0008
LDAA #0x007A
SUB A,D
STA 0x01FB
LD D,#0x008A
LDAA #0x009C
SUB A,D
STA 0x01FC
LD D,#0x007A
LDAA #0x00A1
SUB A,D
STA 0x01FD
LD D,#0x00E9
LDAA #0x00E5
SUB A,D
STA 0x01FE
LD D,#0x00EF
LDAA #0x0096
SUB A,D
STA 0x01FF
LD D,#0x0031
LDAA #0x002F
SUB A,D
TSA
STA 0x0200
LD D,#0x00C2
LDAA #0x00A8
SUB A,D
TSA
STA 0x0201
LD D,#0x00B5
LDAA #0x00BE
SUB A,D
TSA
STA 0x0202
LD D,#0x00F8
LDAA #0x0088
SUB A,D
TSA
STA 0x0203
LD D,#0x00A9
LDAA #0x0088
SUB A,D
TSA
STA 0x0204
LD D,#0x00A8
LDAA #0x0097
SUB A,D
TSA
STA 0x0205
HALT 
