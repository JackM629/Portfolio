LDAA #0x00BB
STA 0xAADD
LDAA #0x00BB
STA 0xAADE
LDAA #0x0022
LD C,#0x004B
CMP A,C
JPA P,J506
LDAA #0x0082
J506:
STA 0x01FB
LDAA #0x001A
LD C,#0x00C5
CMP A,C
JPA P,J508
LDAA #0x005D
J508:
STA 0x01FD
LDAA #0x0073
LD C,#0x00BD
CMP A,C
JPA P,J510
LDAA #0x0078
J510:
STA 0x01FF
LDAA #0x009A
LD C,#0x00C8
CMP A,C
JPA P,J512
LDAA #0x0020
J512:
STA 0x0201
LDAA #0x00B0
LD C,#0x008B
CMP A,C
JPA P,J514
LDAA #0x0021
J514:
STA 0x0203
LDAA #0x00C4
LD C,#0x00EE
CMP A,C
JPA P,J516
LDAA #0x0013
J516:
STA 0x0205
HALT 
data506: dw J506
data508: dw J508
data510: dw J510
data512: dw J512
data514: dw J514
data516: dw J516
