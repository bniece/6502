* = 0
data = $40

LDA #0
STA $40
BRK

.dsb (data - *), $0

