;test_BVC_rel
;Expected outcome:
;	A = 0x2A
LDA #$00
CLV

BVC clear
BRK			; This won't work if BRK is correctly implemented

clear:
LDA #$2A

BRK
