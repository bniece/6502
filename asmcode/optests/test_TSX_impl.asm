;test_TSX_impl
;Expected outcome:
;	X = 0x2A
LDX #$2A

TXS

INX

TSX

BRK
