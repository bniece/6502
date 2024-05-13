* = $00
data = $40

LDA $40	; Get MSD
ASL 		; MSD times 2
STA $42	; Save MSD x 2
ASL 		; x 4
ASL 		; x 8
CLC
ADC $42	; MSD times ten
ADC $41	; Add LSD
STA $42	; Store binary equivalent
BRK
