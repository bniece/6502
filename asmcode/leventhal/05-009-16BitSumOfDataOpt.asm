* = $00
data = $40

LDA #0		; Sum
TAX			; Index
STA $41		; MSBs of Sum
SUMD: CLC	; Do not include carry
ADC $43,X
BCC COUNT
INC $41		; Only add to high byte on carry
COUNT: INX
CPX $42
BNE SUMD
STA $40
BRK

.dsb (data - *), $0
.byte $00,$00,$03,$C8,$FA,$96

; This was an attempt to optimize speed by incrementing
; the MSB of the answer in place with INC, rather than 
; keeping it in Y and using INY, then transfering at the
; end.  It uses 75 cycles, compared to 71 for the book
; version because INC zpg is a longer instruction.
