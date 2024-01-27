* = $00
data = $40

LDA $40	; Get data
CMP #10	; Is data less than 10?
BCC ASCZ	;
ADC #'A'-'9'-2	;	Add offset for letters (carry = 1)
ASCZ:
ADC #'0'			; Add offset for ASCII
STA $41			; Store result
BRK
