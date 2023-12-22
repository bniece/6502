;test_DEC_absX
;Expected outcome:
;	0x0200 = 0x29
LDA #$2A
STA $0200

LDX #$01

DEC $01FF,X

BRK
