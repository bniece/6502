* = $00
data = $40

LDA #0
STA $40		; LSB of sum
STA $41		; MSB of sum
LDX #0		; Index
ASL $42		; Double element count to account for 16-bit addends
SUMD:
CLC
LDA $40		
ADC $43,X
STA $40
INX
LDA $41
ADC $43,X
STA $41
INX
CPX $42
BNE SUMD
BRK

.dsb (data - *), $0
.byte $00,$00,$03,$F1,$28,$1A,$30,$89,$4B
