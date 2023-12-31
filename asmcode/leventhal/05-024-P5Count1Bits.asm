* = $00
data = $40

LDX #8	; Index
LDY #0	; Number of ones
LDA $40
CHK7:
BPL NOT1
INY
NOT1:
DEX
ASL
BNE CHK7
STY $41
BRK

.dsb (data - *), $0
.byte $3B
