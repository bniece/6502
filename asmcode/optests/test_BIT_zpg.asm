;test_BIT_zpg
;Expected outcome:
;	N,V,Z = 1
LDA #$FF
STA $00
LDA #$00
BIT $00

BRK
