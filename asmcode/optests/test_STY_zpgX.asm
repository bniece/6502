;test_STY_zpgX
;Expected outcome:
;	0x00 = 0x2B
;	0x01 = 0x2A
LDX #$01

LDY #$2A
STY $00,X

LDY #$2B
STY $FF,X	; Should wrap back to beginning of page

BRK
