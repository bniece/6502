;test_CLV_impl
;Expected outcome:
;	A = 0x2A
LDA #$7F
ADC #$01
CLV

BVC clear
BRK			; This won't work if BRK is correctly implemented

clear:
LDA #$2A

BRK
