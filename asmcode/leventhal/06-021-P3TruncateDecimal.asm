* = $00
data = $40

LDX #0	; Index
LDY #' '	; Space for replacement
LDA #'.' ; Decimal point for comparison
CHKDP:
CMP $41,X
BEQ FOUND
INX
CPX $40
BNE CHKDP
BRK
FOUND:
INX
CPX $40
BEQ DONE
STY $41,X
JMP FOUND
DONE:
BRK
