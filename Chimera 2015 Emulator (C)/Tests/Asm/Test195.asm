LDAA #0x00BB
STA 0xAADD
LDAA #0x00BB
STA 0xAADE
LDAA #0x000E
LD C,#0x005A
CMP A,C
JMI P,J506
LDAA #0x00E6
J506:
STA 0x01FB
LDAA #0x0035
LD C,#0x0095
CMP A,C
JMI P,J508
LDAA #0x0047
J508:
STA 0x01FD
LDAA #0x006B
LD C,#0x009A
CMP A,C
JMI P,J510
LDAA #0x00E9
J510:
STA 0x01FF
LDAA #0x0034
LD C,#0x009C
CMP A,C
JMI P,J512
LDAA #0x00FC
J512:
STA 0x0201
LDAA #0x002C
LD C,#0x00E2
CMP A,C
JMI P,J514
LDAA #0x0007
J514:
STA 0x0203
LDAA #0x002B
LD C,#0x0025
CMP A,C
JMI P,J516
LDAA #0x00EE
J516:
STA 0x0205
HALT 
data506: dw J506
data508: dw J508
data510: dw J510
data512: dw J512
data514: dw J514
data516: dw J516
