;test_LDA_zpgX
;Expected outcome:
;	A = 0x2B
;	0x10 = 0x2A
;	0x11 = 0x2B
LDX #$01

LDY #$2A
STY $01
LDA $00,X
STA $10

LDY #$2B
STY $00
LDA $FF,X	; Should wrap back to beginning of zero page
STA $11

BRK
