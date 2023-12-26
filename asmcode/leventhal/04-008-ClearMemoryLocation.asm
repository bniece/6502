* = 0
data = $40

LDA #0
STA $41
BRK

.dsb (data - *), $0

