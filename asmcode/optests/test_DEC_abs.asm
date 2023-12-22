;test_DEC_abs
;Expected outcome:
;	0x0200 = 0x29
LDX #$2A
STX $0200
DEC $0200

BRK
