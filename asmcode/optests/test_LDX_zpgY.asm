;test_LDX_zpgY
;Expected outcome:
;	X = 0x2B
;	0x10 = 0x2A
;	0x11 = 0x2B
LDY #$01

LDA #$2A
STA $01
LDX $00,Y
STX $10

LDA #$2B
STA $00
LDX $FF,Y	; Should wrap back to beginning of zero page
STX $11

BRK
