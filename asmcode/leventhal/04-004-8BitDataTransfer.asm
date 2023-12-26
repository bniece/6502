* = $00
data = $40

LDA $40
STA $41
BRK

.dsb (data - *), $0
.byte $6A
