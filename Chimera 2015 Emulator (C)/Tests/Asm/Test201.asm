LDAA #0x00BB
STA 0xAADD
LDAA #0x00BB
STA 0xAADE
LDAA #0x00F1
LD C,#0x00E5
CMP A,C
JPE J506
LDAA #0x004A
J506:
STA 0x01FB
LDAA #0x0027
LD C,#0x002D
CMP A,C
JPE J508
LDAA #0x006A
J508:
STA 0x01FD
LDAA #0x00F2
LD C,#0x00AB
CMP A,C
JPE J510
LDAA #0x00E0
J510:
STA 0x01FF
LDAA #0x0010
LD C,#0x004D
CMP A,C
JPE J512
LDAA #0x0041
J512:
STA 0x0201
LDAA #0x00F8
LD C,#0x0062
CMP A,C
JPE J514
LDAA #0x0046
J514:
STA 0x0203
LDAA #0x00BD
LD C,#0x0030
CMP A,C
JPE J516
LDAA #0x0037
J516:
STA 0x0205
HALT 
data506: dw J506
data508: dw J508
data510: dw J510
data512: dw J512
data514: dw J514
data516: dw J516
