;test_INC_absX
;Expected outcome:
;	0x0200 = 0x2B
LDA #$2A
STA $0200

LDX #$01

INC $01FF,X

BRK
