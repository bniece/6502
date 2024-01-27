* = $00
data = $40
SSEG = $20

LDA #0	; Get error code to blank display
LDX $41
CPX #10	; Is data a decimal digit
BCS DONE	; No, keep error code
LDA SSEG,X	; Yes, get code from table
DONE:
STA $42
BRK

.dsb (SSEG - *), $0
.byt $3F,$06,$5B,$4F,$66
.byt $6D,$7D,$07,$7F,$6F
