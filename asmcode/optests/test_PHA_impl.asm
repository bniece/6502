;test_PHA_impl
;Expected outcome:
;	0x01FF = 0x2A
;	SP = 0xFE
LDX #$FF
TXS

LDA #$2A
PHA

BRK
