* = 0
data = $40

LDA $40
ASL
ASL
STA $41
BRK

.dsb (data - *), $0
.byt $5D
