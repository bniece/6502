* = $00
data = $40

LDX $41		; Element count
LDA #0		; Maximum
MAXM:
CMP $41,X
BCS NOCHG
LDA $41,X
NOCHG:
DEX
BNE MAXM
STA $40
BRK

.dsb (data - *), $0
.byte $00,$05,$67,$79,$15,$E3,$72
