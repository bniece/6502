* = 0
data = $40
sqtab = $50

CLC
LDX $40
LDA sqtab,X
STA $42
LDX $41
LDA sqtab,X
ADC $42
STA $42
BRK

.dsb (data - *), $0
.byte 03,06
.dsb (sqtab - *), $0
.byt 0,1,4,9,16,25,36,49
