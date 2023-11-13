;test_LDA_absY
;Expected outcome:
;	A = 0x2A
LDY #$01

LDX #$2A
STX $0201

LDA $0200,Y

BRK
