;test_PLP_impl
;Expected outcome:
;	Flags = NvBDIzc
;	SP = 0xFF
SED
CLC

LDX #$FF
TXS

PHP

CLD
LDA #$00

PLP

BRK
