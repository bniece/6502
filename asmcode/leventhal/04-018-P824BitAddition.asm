* = 0
data = $40

CLC
LDA $40
ADC $43
STA $46
LDA $41
ADC $44
STA $47
LDA $42
ADC $45
STA $48
BRK

.dsb (data - *), $0
.byt $2A,$67,$35,$F8,$A4,$51
