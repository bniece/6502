;test_RTS_impl
;Expected outcome:
;	A = 0x2B
* = $0600

JSR seta
LDA #$2B
BRK

seta:
LDA #$2A
RTS

BRK
