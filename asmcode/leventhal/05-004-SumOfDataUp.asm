* = $00
data = $40

LDA #0		; Sum
TAX			; Index
SUMD: CLC	; Do not include carry
ADC $42,X
INX
CPX $41
BNE SUMD
STA $40
BRK

.dsb (data - *), $0
.byte $00,$03,$28,$55,$26
