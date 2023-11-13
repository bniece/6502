;test_LDX_abs
;Expected outcome:
;	X = 0x2A
LDA #$2A
STA $0200

LDX $0200

BRK
