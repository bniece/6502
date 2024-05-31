* = $00
data = $42

LDX #0		; Count bits
STX $41		; Clear destination
LDY #$FF		; Error code
CONV:
ASL $41		; Make room for new bit
LDA data,X	; Get data
SEC
SBC #'0'		; Convert ASCII to number
CMP #2		;
BPL ERROR	; Error if > 1
CLC
ADC $41		; Add to result
STA $41		; and replace
INX
CPX #8
BNE CONV		; Do next bit if not 8
BRK
ERROR:
STY $40
BRK

.dsb (data - *), $0
.byte $D2
