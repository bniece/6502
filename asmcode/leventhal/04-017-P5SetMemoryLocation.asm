* = 0
data = $40

LDA #$FF
STA $40
BRK

.dsb (data - *), $0
