;test_SEC_impl
;Expected outcome:
;	A = 0x2A
CLC
SEC

BCS clear
BRK			; This won't work if BRK is correctly implemented

clear:
LDA #$2A

BRK
