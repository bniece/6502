;test_BRK_impl
;Expected outcome:
;	A = 0x2A
LDA #$2A

BRK			; This won't work if BRK is correctly implemented

clear:
LDA #$00

BRK
