* = $00
data = $40

LDX #0	; String length
LDA #$0D	; \r for comparison
CHKCR:
CMP $41,X
BEQ DONE
INX
JMP CHKCR
DONE:
STX $40
BRK
