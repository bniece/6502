;test_SBC_xpgX_BCD
;Expected outcome:
;	A C
;	0200: 41 01
;	0210: 00 01
;	0220: 99 00
SED

LDA #$42
STA $00
LDA #$83
STA $01
LDA #$84
STA $02

LDX #$00

LDA #$83
SEC
SBC $00,X
STA $0200
BCC noC1	; Save C flag
LDA #$01
STA $0201
noC1:

INX

LDA #$83
SEC
SBC $00,X
STA $0210
BCC noC2	; Save C flag
LDA #$01
STA $0211
noC2:

INX

LDA #$83
SEC
SBC $00,X
STA $0220
BCC noC3	; Save C flag
LDA #$01
STA $0221
noC3:

BRK
