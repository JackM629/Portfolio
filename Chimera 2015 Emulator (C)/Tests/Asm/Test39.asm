LDZ #0x342A
LDAA #0x00E3
TAP  
LDAA #0x00BB
STA 0xAADD
LDAA #0x00BB
STA 0xAADE
LD D,#0x00B8
LDAA #0x0074
CMP A,D
STA 0x01FA
LD D,#0x00F8
LDAA #0x00E6
CMP A,D
STA 0x01FB
LD D,#0x006C
LDAA #0x00B0
CMP A,D
STA 0x01FC
LD D,#0x00DD
LDAA #0x009F
CMP A,D
STA 0x01FD
LD D,#0x00D2
LDAA #0x0026
CMP A,D
STA 0x01FE
LD D,#0x000A
LDAA #0x002F
CMP A,D
STA 0x01FF
LD D,#0x0087
LDAA #0x0017
CMP A,D
TSA
STA 0x0200
LD D,#0x00FA
LDAA #0x00B3
CMP A,D
TSA
STA 0x0201
LD D,#0x0089
LDAA #0x0054
CMP A,D
TSA
STA 0x0202
LD D,#0x002D
LDAA #0x00AB
CMP A,D
TSA
STA 0x0203
LD D,#0x0097
LDAA #0x0068
CMP A,D
TSA
STA 0x0204
LD D,#0x00C7
LDAA #0x0045
CMP A,D
TSA
STA 0x0205
HALT 