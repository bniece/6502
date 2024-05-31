* = $00
data = $40
SSEG = $20

LDA #$FF	; Error code
LDY #$09
NEXT:
LDX SSEG,Y
CPX $40
BEQ DONE
DEY
BNE NEXT
DEY
DONE:
STY $41
BRK

.dsb (SSEG - *), $0
.byt $3F,$06,$5B,$4F,$66
.byt $6D,$7D,$07,$7F,$6F
