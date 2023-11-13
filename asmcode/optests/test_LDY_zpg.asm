;test_LDY_zpg
;Expected outcome:
;	Y = 0x2A
LDA #$2A
STA $00

LDY $00

BRK
