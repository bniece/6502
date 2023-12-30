* = $00
data = $40

LDA #0		; Sum
LDY #0		; Number of negatives
SRNEG:
LDA $42,X
BPL CHCNT
INY
CHCNT:
INX
CPX $41
BNE SRNEG
STY $40
BRK

.dsb (data - *), $0
.byte $00,$06,$68,$F2,$87,$30,$59,$2A
