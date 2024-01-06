* = $00
data = $40

LDX #0	; Index
LDY #' '	; Space for replacement
LDA #'0' ; Zero for comparison
CHKZ:
CMP $41,X
BNE DONE
STY $41,X
INX
CPX $40
BNE CHKZ
DONE:
BRK
