LDZ #0x342A
LDAB #0x00E3
TAP  
LDAB #0x00BB
STB 0xAADD
LDAB #0x00BB
STB 0xAADE
LD C,#0x00B9
LDAB #0x0074
ADC B,C
STB 0x01FA
LD C,#0x00AD
LDAB #0x0009
ADC B,C
STB 0x01FB
LD C,#0x0099
LDAB #0x0073
ADC B,C
STB 0x01FC
LD C,#0x0043
LDAB #0x00D5
ADC B,C
STB 0x01FD
LD C,#0x0073
LDAB #0x0041
ADC B,C
STB 0x01FE
LD C,#0x000D
LDAB #0x0085
ADC B,C
STB 0x01FF
LD C,#0x0049
LDAB #0x00CE
ADC B,C
TSA
STA 0x0200
LD C,#0x00A2
LDAB #0x00D0
ADC B,C
TSA
STA 0x0201
LD C,#0x00EC
LDAB #0x00AE
ADC B,C
TSA
STA 0x0202
LD C,#0x006F
LDAB #0x0024
ADC B,C
TSA
STA 0x0203
LD C,#0x002B
LDAB #0x0061
ADC B,C
TSA
STA 0x0204
LD C,#0x0037
LDAB #0x000D
ADC B,C
TSA
STA 0x0205
HALT 
