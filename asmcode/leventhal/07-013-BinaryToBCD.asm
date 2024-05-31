* = $00
data = $40

LDA $40	; Get Data
LDX #$FF	; number of subtractions = tens
NEXT:
INX
SEC
SBC #10
BPL NEXT	; if still > 0, subtract more
STX $41
CLC
ADC #10	; Remainder is the ones - 10
STA $42
BRK
