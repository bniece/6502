;test_BNE_rel
;Expected outcome:
;	A = 0x2A
LDA #$01

BNE notzero
BRK			; This won't work if BRK is correctly implemented

notzero:
LDA #$2A

BRK
