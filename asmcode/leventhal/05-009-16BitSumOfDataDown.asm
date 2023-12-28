* = $00
data = $40

LDA #0		; Sum
LDX $42		; Index
TAY			; MSBs of Sum
SUMD: CLC	; Do not include carry
ADC $42,X
BCC COUNT
INY			; Only add to high byte on carry
COUNT: DEX
BNE SUMD
STA $40
STY $41
BRK

.dsb (data - *), $0
.byte $00,$00,$03,$C8,$FA,$96

; This version counts down through the list
; instead of up.  It doesn't need to do teh CPX
; and so saves a few cycles, taking 63 instead of
; 71 for the book version.
