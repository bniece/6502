;test_PLP_impl
;Expected outcome:
;	Flags = NvbDIzc
;	SP = 0xFF
SED

LDX #$FF
TXS

PHP

CLD
LDA #$00

PLP

BRK
