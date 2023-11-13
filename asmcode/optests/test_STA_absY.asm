;test_STA_absY
;Expected outcome:
;	0x0200 = 0x2B
;	0x0201 = 0x2A
LDY #$01

LDA #$2A
STA $0200,Y

LDA #$2B
STA $01FF,Y		; Should cross page boundary

BRK
