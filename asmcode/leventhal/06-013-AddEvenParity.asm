* = $00
data = $40

LDX $40		; Index
GETDATA:
LDY #0		; Bit count
LDA $40,X	; Array element to check
CHBIT:		; Check bit 7, count if set
BPL CHKZ
INY
CHKZ:			; Check if any more bits set
ASL
BNE CHBIT
TYA			; If none, finish this one up
LSR			; move LSB to carry
BCC NEXTE	; Do next one if it was even
LDA $40,X	; or set parity bit if it was odd
ORA #%10000000
STA $40,X
NEXTE:
DEX
BNE GETDATA
BRK

.dsb (data - *), $0
.byte $06,$31,$32,$33,$34,$35,$36
