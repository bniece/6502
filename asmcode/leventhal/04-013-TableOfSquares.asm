* = 0
sqtab = $50

CLC
LDX $41
LDA sqtab,X
STA $42
BRK

.dsb (sqtab - *), $0
.byt 0,1,4,9,16,25,36,49
