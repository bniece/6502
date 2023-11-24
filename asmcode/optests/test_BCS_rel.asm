;test_BCS_rel
;Expected outcome:
;	A = 0x2A
LDA #$00
SEC

BCS set
BRK			; This won't work if BRK is correctly implemented

set:
LDA #$2A

BRK
