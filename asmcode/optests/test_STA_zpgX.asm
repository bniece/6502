;test_STA_zpgX
;Expected outcome:
;	0x00 = 0x2B
;	0x01 = 0x2A
LDX #$01

LDA #$2A
STA $00,X

LDA #$2B
STA $FF,X	; Should wrap back to beginning of page

BRK
