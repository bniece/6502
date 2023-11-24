;test_BEQ_rel
;Expected outcome:
;	A = 0x2A
LDA #$00

BEQ zero
BRK			; This won't work if BRK is correctly implemented

zero:
LDA #$2A

BRK
