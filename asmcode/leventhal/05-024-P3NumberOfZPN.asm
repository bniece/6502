* = $00
data = $40

LDA #0 		; Initialize counts
STA $40		; Negatives
STA $41		; Zeroes
STA $42		; Posititives
LDX #0		; Index
NEXT:
LDA $44,X
BPL NOTNEG	; Check for negative
INC $40
JMP CHCNT
NOTNEG:
BNE NOTZER	; Check for zero
INC $41
JMP CHCNT
NOTZER:
INC $42		; must be positivie
CHCNT:
INX
CPX $43
BNE NEXT
BRK

.dsb (data - *), $0
.byte $00,$00,$00,$06,$68,$F2,$87,$00,$59,$2A
