* = $00
data = $40

LDA $40
STA $42
LDA $41
STA $43
BRK

.dsb (data - *), $0
.byte $3E,$B7
