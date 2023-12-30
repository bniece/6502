* = $00
data = $40

LDY $41		; Element count
LDA #0		; Maximum
MAXM:
CMP ($42),Y
BCS NOCHG
LDA ($42),Y
NOCHG:
DEY
BNE MAXM
STA $40
BRK

.dsb (data - *), $0
.byte $00,$05,$43,$00,$67,$79,$15,$E3,$72
