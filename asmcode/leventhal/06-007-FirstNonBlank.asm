* = $00
data = $40

LDX #0	; String length
LDA #' '	; Space for comparison
CHBLK:
CMP $42,X
BNE DONE
INX
JMP CHBLK
DONE:
STX $40
BRK
