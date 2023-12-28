* = 0
data = $40

LDA $40
ASL
ASL
ASL
ASL
STA $42
LDA $41
AND #%00001111
CLC
ADC $42
STA $42
BRK

.dsb (data - *), $0
.byt $6A,$B3
