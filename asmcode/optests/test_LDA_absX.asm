;test_LDA_absX
;Expected outcome:
;	A = 0x2A
LDX #$01

LDY #$2A
STY $0201

LDA $0200,X

BRK
