* = $00
data = $40

LDX $41		; Index
GETDATA:
LDY #0		; Bit count
LDA $42,X	; Array element to check
CHBIT:		; Check bit 7, count if set
BPL CHKZ
INY
CHKZ:			; Check if any more bits set
ASL
BNE CHBIT
TYA			; If none, finish this one up
LSR			; move LSB to carry
BCC NEXTE	; Do next one if it was even
LDA #$FF		; Otherwise, store error flag
STA $40
BRK			; and quit
NEXTE:
DEX
BNE GETDATA
BRK
