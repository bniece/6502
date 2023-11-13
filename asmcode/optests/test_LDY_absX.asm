;test_LDY_absX
;Expected outcome:
;	Y = 0x2A
LDX #$01

LDA #$2A
STA $0201

LDY $0200,X

BRK
