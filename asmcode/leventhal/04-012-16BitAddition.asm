* = 0
data = $40

CLC
LDA $40
ADC $42
STA $44
LDA $41
ADC $43
STA $45
BRK

.dsb (data - *), $0
.byt $2A,$67,$F8,$14
