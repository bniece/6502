;test_PLA_impl
;Expected outcome:
;	A = 0x2A
;	SP = 0xFF
LDX #$FF
TXS

LDA #$2A
PHA

TXA

PLA

BRK
