;test_STA_Xind
;Expected outcome:
;	0x0200 = 0x2A
;	0x0201 = 0x2B
LDX #$01

LDA #$00
STA $01
LDA #$02
STA $02

LDA #$2A
STA ($00,X)

LDA #$01
STA $FF
LDA #$02
STA $00

LDA #$2B
STA ($FE,X)		; high address byte at 0x00

BRK
