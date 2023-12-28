* = $00
data = $40

LDA #0		; Sum
TAX			; Index
TAY			; MSBs of Sum
SUMD: CLC	; Do not include carry
ADC $43,X
BCC COUNT
INY			; Only add to high byte on carry
COUNT: INX
CPX $42
BNE SUMD
STA $40
STY $41
BRK

.dsb (data - *), $0
.byte $00,$00,$03,$C8,$FA,$96
