* = $00
data = $40

LDA $40
EOR #%11111111		; Take the ones complement
CLC
ADC #$01				; Add one to get the twos complement
STA $41
BRK

.dsb (data - *), $0
.byte $3E

; This version is 9 bytes of code and takes 19 cycles.
; It is worse on both counts.
