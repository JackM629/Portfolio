LDAA #0x00BB
STA 0xAADD
LDAA #0x00BB
STA 0xAADE
LDAA #0x0014
LD C,#0x002A
CMP A,C
JCC P,J506
LDAA #0x0000
J506:
STA 0x01FB
LDAA #0x00A2
LD C,#0x00A7
CMP A,C
JCC P,J508
LDAA #0x003C
J508:
STA 0x01FD
LDAA #0x0059
LD C,#0x00FC
CMP A,C
JCC P,J510
LDAA #0x0099
J510:
STA 0x01FF
LDAA #0x00BC
LD C,#0x0097
CMP A,C
JCC P,J512
LDAA #0x004B
J512:
STA 0x0201
LDAA #0x00A4
LD C,#0x00EC
CMP A,C
JCC P,J514
LDAA #0x00D4
J514:
STA 0x0203
LDAA #0x006B
LD C,#0x0002
CMP A,C
JCC P,J516
LDAA #0x00ED
J516:
STA 0x0205
HALT 
data506: dw J506
data508: dw J508
data510: dw J510
data512: dw J512
data514: dw J514
data516: dw J516