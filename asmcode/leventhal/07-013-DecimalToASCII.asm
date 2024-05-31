* = $00
data = $40

LDX #$20		; Error code
LDA $40
CMP #09		; Is data <= 9?
BPL DONE
CLC
ADC #'0'		; Convert to ASCII
TAX			; Save digit if valid
DONE:
STX $41
BRK
