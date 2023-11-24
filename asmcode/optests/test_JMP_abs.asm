;test_JMP_abs
;Expected outcome:
;	A = 0x2A
* = $0600

LDA #$00

JMP seta
BRK


seta:
LDA #$2A

BRK
