LDAA #0x00BB
STA 0xAADD
LDAA #0x00BB
STA 0xAADE
LODS #0xAA54
LDAA #0x0007
LD C,#0x001F
CMP A,C
JPR J506
LDAA #0x00F0
PUSH A
PUSH A
J506:
POP A
STA 0x01FB
LDAA #0x0074
LD C,#0x0021
CMP A,C
JPR J508
LDAA #0x00F9
PUSH A
PUSH A
J508:
POP A
STA 0x01FD
LDAA #0x0004
LD C,#0x00BD
CMP A,C
JPR J510
LDAA #0x00B1
PUSH A
PUSH A
J510:
POP A
STA 0x01FF
LDAA #0x0059
LD C,#0x008E
CMP A,C
JPR J512
LDAA #0x00CF
PUSH A
PUSH A
J512:
POP A
STA 0x0201
LDAA #0x0084
LD C,#0x00F5
CMP A,C
JPR J514
LDAA #0x0065
PUSH A
PUSH A
J514:
POP A
STA 0x0203
LDAA #0x007F
LD C,#0x0078
CMP A,C
JPR J516
LDAA #0x0001
PUSH A
PUSH A
J516:
POP A
STA 0x0205
HALT 
data506: dw J506
data508: dw J508
data510: dw J510
data512: dw J512
data514: dw J514
data516: dw J516
