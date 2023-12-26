* = 0
data = $40

LDA $40
AND #%00001111
STA $42
LDA $40
LSR
LSR
LSR
LSR
STA $41
BRK

.dsb (data - *), $0
.byt $3F
