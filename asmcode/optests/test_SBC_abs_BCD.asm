;test_SBC_abs_BCD
;Expected outcome:
;	A C
;	0200: 41 01
;	0210: 00 01
;	0220: 99 00
SED

LDA #$42
STA $02F0

LDA #$83
SEC
SBC $02F0
STA $0200
BCC noC1	; Save C flag
LDA #$01
STA $0201
noC1:

LDA #$42
SEC
SBC $02F0
STA $0210
BCC noC2	; Save C flag
LDA #$01
STA $0211
noC2:

LDA #$41
SEC
SBC $02F0
STA $0220
BCC noC3	; Save C flag
LDA #$01
STA $0221
noC3:

BRK
