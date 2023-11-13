;test_LDY_zpgX
;Expected outcome:
;	Y = 0x2B
;	0x10 = 0x2A
;	0x11 = 0x2B
LDX #$01

LDA #$2A
STA $01
LDY $00,X
STY $10

LDA #$2B
STA $00
LDY $FF,X	; Should wrap back to beginning of zero page
STY $11

BRK
