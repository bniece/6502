* = $00
data = $41

LDA $41	; Get Data
LDX #8	; number of bits = 8
LDY #'0'	; ASCII zero to store in string
CONV:
STY $41,X	; Store ASCII zero in string
LSR			; Is next bit of data zero?
BCC COUNT
INC $41,X	; No - make string element ASCII one
COUNT:
DEX
BNE CONV
BRK

.dsb (data - *), $0
.byte $D2
