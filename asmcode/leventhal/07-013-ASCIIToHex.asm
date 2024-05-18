* = $00
data = $40

LDA $40	; Get data
SEC
SBC #'0' ; Subtract ASCII zero
CMP #10	; Is result less than 10?
BCC STORE	; Done
SBC #7
;SBC #'A'-'9'-1	;	Add offset for letters (carry = 0)
STORE:
STA $41			; Store result
BRK
