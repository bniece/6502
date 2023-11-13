;test_LDX_zpg
;Expected outcome:
;	X = 0x2A
LDA #$2A
STA $00

LDX $00

BRK
