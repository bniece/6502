* = $00
data = $40

LDX $41		; Element count
LDA $41,X	; Minimum - seed with first value
MINM:
CMP $41,X
BCC NOCHG
LDA $41,X
NOCHG:
DEX
BNE MINM
STA $40
BRK

.dsb (data - *), $0
.byte $00,$05,$67,$79,$15,$E3,$72
