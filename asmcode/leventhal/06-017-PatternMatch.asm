* = $00
data = $40

LDX #0	; Index
LDY #$FF	; Marker for no match
CHCAR:
LDA $42,X	; Character from string 1
CMP $52,X	; Compare to string 2
BNE DONE
INX
CPX $41
BNE CHCAR
LDY #0		; none left = match
DONE:
STY $40
BRK
