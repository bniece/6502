;test_INC_zpg
;Expected outcome:
;	0x00 = 0x2B
LDX #$2A
STX $00
INC $00

BRK
