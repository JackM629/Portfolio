LDAA #0x00BB
STA 0xAADD
LDAA #0x00BB
STA 0xAADE
LODS #0xAA54
LDAA #0x007C
LD C,#0x00C1
CMP A,C
JPR P,J506
LDAA #0x002B
PUSH A
PUSH A
J506:
POP A
STA 0x01FB
LDAA #0x0083
LD C,#0x0062
CMP A,C
JPR P,J508
LDAA #0x00E2
PUSH A
PUSH A
J508:
POP A
STA 0x01FD
LDAA #0x0052
LD C,#0x0091
CMP A,C
JPR P,J510
LDAA #0x00B1
PUSH A
PUSH A
J510:
POP A
STA 0x01FF
LDAA #0x00AF
LD C,#0x00F1
CMP A,C
JPR P,J512
LDAA #0x0043
PUSH A
PUSH A
J512:
POP A
STA 0x0201
LDAA #0x0034
LD C,#0x00D8
CMP A,C
JPR P,J514
LDAA #0x00A7
PUSH A
PUSH A
J514:
POP A
STA 0x0203
LDAA #0x004D
LD C,#0x00EB
CMP A,C
JPR P,J516
LDAA #0x0072
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
