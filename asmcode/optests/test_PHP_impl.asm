;test_PHP_impl
;Expected outcome:
;	0x01FF = 0xBC
;	SP = 0xFE
CEC
SED

LDX #$FF
TXS

PHP

BRK
