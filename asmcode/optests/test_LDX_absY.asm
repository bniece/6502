;test_LDX_absY
;Expected outcome:
;	X = 0x2A
LDY #$01

LDA #$2A
STA $0201

LDX $0200,Y

BRK
