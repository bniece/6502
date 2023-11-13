;test_LDA_abs
;Expected outcome:
;	A = 0x2A
LDX #$2A
STX $0200

LDA $0200

BRK
