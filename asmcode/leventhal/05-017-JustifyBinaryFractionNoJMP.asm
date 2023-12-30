* = $00
data = $40

LDY #0	; Number of shifts
LDA $40
BEQ DONE	; Quit if data is zero
CHKMS:
INY
ASL
BCC CHKMS
ROR
DEY
DONE:
STA $41
STY $42:
BRK
