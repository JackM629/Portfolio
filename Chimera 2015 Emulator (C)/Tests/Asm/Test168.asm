LDAA #0x00BB
STA 0xAADD
LDAA #0x00BB
STA 0xAADE
LDAA #0x0080
LD C,#0x0088
CMP A,C
JCC J506
LDAA #0x0089
J506:
STA 0x01FB
LDAA #0x00E6
LD C,#0x008E
CMP A,C
JCC J508
LDAA #0x0067
J508:
STA 0x01FD
LDAA #0x00E0
LD C,#0x0098
CMP A,C
JCC J510
LDAA #0x0072
J510:
STA 0x01FF
LDAA #0x00E7
LD C,#0x001B
CMP A,C
JCC J512
LDAA #0x00AA
J512:
STA 0x0201
LDAA #0x00A8
LD C,#0x00FE
CMP A,C
JCC J514
LDAA #0x00FA
J514:
STA 0x0203
LDAA #0x004E
LD C,#0x0024
CMP A,C
JCC J516
LDAA #0x001C
J516:
STA 0x0205
HALT 
data506: dw J506
data508: dw J508
data510: dw J510
data512: dw J512
data514: dw J514
data516: dw J516