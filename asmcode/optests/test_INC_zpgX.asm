;test_INC_zpgX
;Expected outcome:
;	0x00 = 0x2B
LDA #$2A
STA $00

LDX #$01

INC $FF,X

BRK
