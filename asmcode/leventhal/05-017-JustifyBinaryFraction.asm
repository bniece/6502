* = $00
data = $40

LDY #0	; Number of shifts
LDA $40
BEQ DONE	; Quit if data is zero
CHKMS:
BMI DONE
INY
ASL
JMP CHKMS
DONE:
STA $41
STY $42:
BRK
