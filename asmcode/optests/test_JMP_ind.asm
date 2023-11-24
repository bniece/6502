;test_JMP_ind
;Expected outcome:
;	A = 0x2A
* = $0600

LDA #$00

LDX #$20
STX $0200
LDX #$06
STX $0201

JMP ($0200)
BRK

.dsb $0620-*,$00
LDA #$2A

BRK
