* = 0
data = $40

LDA $40
AND #%11110000
STA $41
BRK

.dsb (data - *), $0
.byt $C4
