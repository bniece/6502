;test_BIT_abs
;Expected outcome:
;	N,V,Z = 1
LDA #$FF
STA $0200
LDA #$00
BIT $0200

BRK
