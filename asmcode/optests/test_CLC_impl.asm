;test_CLC_impl
;Expected outcome:
;	A = 0x2A
LDA #$FF
CLC
ADC #$01
CLC

BCC clear
BRK			; This won't work if BRK is correctly implemented

clear:
LDA #$2A

BRK
