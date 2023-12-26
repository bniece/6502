* = 0
data = $40

LDA $40
AND #%00001111
STA $41
BRK

.dsb (data - *), $0
.byt $3D
