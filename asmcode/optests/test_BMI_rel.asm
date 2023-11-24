;test_BMI_rel
;Expected outcome:
;	A = 0x2A
LDA #$80

BMI minus
BRK			; This won't work if BRK is correctly implemented

minus:
LDA #$2A

BRK
