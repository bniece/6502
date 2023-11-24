;test_BPL_rel
;Expected outcome:
;	A = 0x2A
LDA #$01

BPL plus
BRK			; This won't work if BRK is correctly implemented

plus:
LDA #$2A

BRK
