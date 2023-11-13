;test_STA_absX
;Expected outcome:
;	0x0200 = 0x2B
;	0x0201 = 0x2A
LDX #$01

LDA #$2A
STA $0200,X

LDA #$2B
STA $01FF,X		; Should cross page boundary

BRK
