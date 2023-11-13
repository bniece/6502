;test_LDY_abs
;Expected outcome:
;	Y = 0x2A
LDA #$2A
STA $0200

LDY $0200

BRK
