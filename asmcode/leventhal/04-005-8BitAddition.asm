* = 0
data = $40

CLC
LDA $40
ADC $41 
STA $42
BRK

.dsb (data - *), $0
.byt $38,$2B
