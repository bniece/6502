;test_LDA_Xind
;Expected outcome:
;	A = 0x2B
;	0x10 = 0x2A
;	0x11 = 0x2B
LDX #$01

LDY #$00
STY $01
LDY #$02
STY $02
LDY #$2A
STY $0200

LDA ($00,X)
STA $10

LDY #$01
STY $FF
LDY #$02
STY $00
LDY #$2B
STY $0201

LDA ($FE,X)		; high address byte at 0x00
STA $11

BRK
