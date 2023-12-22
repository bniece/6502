;test_DEC_zpgX
;Expected outcome:
;	0x00 = 0x29
LDA #$2A
STA $00

LDX #$01

DEC $FF,X

BRK
