* = $00
data = $40

LDX #$FF		; Error code
LDA $40
SEC
SBC #'0'		; Is data below ASCII 0?
BCC DONE
CMP #10		; Is data abvoe ASCII 9?
BCS DONE
TAX			; Save digit if valid
DONE:
STX $41
BRK
