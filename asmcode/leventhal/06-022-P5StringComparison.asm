* = $00
data = $40

LDX #0	; Index
LDY #$FF	; Marker for second string greater
CHCAR:
LDA $42,X	; Character from string 1
CMP $52,X	; Compare to string 2
BMI DONE
BNE CHGMK
INX
CPX $41
BNE CHCAR
CHGMK:		; equal or less than
LDY #0		; none left = match
DONE:
STY $40
BRK
