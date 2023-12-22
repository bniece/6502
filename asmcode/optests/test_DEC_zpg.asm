;test_DEC_zpg
;Expected outcome:
;	0x00 = 0x29
LDX #$2A
STX $00
DEC $00

BRK
