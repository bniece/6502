;test_PHP_impl
;Expected outcome:
;	0x01FF = 0x8C
;	SP = 0xFE
SED

LDX #$FF
TXS

PHP

BRK
