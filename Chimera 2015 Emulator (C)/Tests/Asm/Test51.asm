LDZ #0x342A
LDAA #0x00E3
TAP  
LDAA #0x00BB
STA 0xAADD
LDAA #0x00BB
STA 0xAADE
LD D,#0x0093
LDAA #0x00C7
EOR A,D
STA 0x01FA
LD D,#0x0091
LDAA #0x0070
EOR A,D
STA 0x01FB
LD D,#0x0042
LDAA #0x0095
EOR A,D
STA 0x01FC
LD D,#0x0090
LDAA #0x0030
EOR A,D
STA 0x01FD
LD D,#0x002E
LDAA #0x00D4
EOR A,D
STA 0x01FE
LD D,#0x00F4
LDAA #0x0088
EOR A,D
STA 0x01FF
LD D,#0x004A
LDAA #0x00DF
EOR A,D
TSA
STA 0x0200
LD D,#0x00C5
LDAA #0x00AB
EOR A,D
TSA
STA 0x0201
LD D,#0x0052
LDAA #0x00C7
EOR A,D
TSA
STA 0x0202
LD D,#0x0018
LDAA #0x0077
EOR A,D
TSA
STA 0x0203
LD D,#0x0060
LDAA #0x00C5
EOR A,D
TSA
STA 0x0204
LD D,#0x0009
LDAA #0x004C
EOR A,D
TSA
STA 0x0205
HALT 
