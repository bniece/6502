* = $00
data = $40

LDX #$FF	; String length
LDA #' '	; Space for comparison
CHBLK:
INX
CMP $42,X
BEQ CHBLK
STX $40
BRK
