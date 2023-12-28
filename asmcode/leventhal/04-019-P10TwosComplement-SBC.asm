* = $00
data = $40

LDA #$00
SEC
SBC $40	; Subtract data from 0 to get twos complement
STA $41
BRK

.dsb (data - *), $0
.byte $3E

; This version is 7 bytes of code and takes 17 cycles to run.
; It is better on both counts.
