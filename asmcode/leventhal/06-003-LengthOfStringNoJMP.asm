* = $00
data = $40

LDX #$FF	; String length
LDA #$0D	; \r for comparison
CHKCR:
INX
CMP $41,X
BNE CHKCR
STX $40
BRK

; In addition to not using the absolute JMP address,
; this version does the 6-character string in 76 cycles
; vs. 87 for the original.
; The program is also only 12 bytes long instead of 15
