* = $00
data = $40

SED
CLC
LDA $40
ADC #$90	; Develop extra 6 and carry
ADC #$40 ; Add in carry, ASCII offset
STA $41
CLD
BRK
