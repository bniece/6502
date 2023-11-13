;test_STX_zpgY
;Expected outcome:
;	0x00 = 0x2B
;	0x01 = 0x2A
LDY #$01

LDX #$2A
STX $00,Y

LDX #$2B
STX $FF,Y	; Should wrap back to beginning of page

BRK
