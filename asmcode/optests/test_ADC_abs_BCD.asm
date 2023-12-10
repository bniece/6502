;test_ADC_abs_BCD
;Expected outcome:
;	A C
;	0200: 43 00
;	0210: 00 01
;	0220: 01 01
SED

LDA #$42
STA $02F0

LDA #$01
CLC
ADC $02F0
STA $0200
BCC noC1	; Save C flag
LDA #$01
STA $0201
noC1:

LDA #$58
CLC
ADC $02F0
STA $0210
BCC noC2	; Save C flag
LDA #$01
STA $0211
noC2:

LDA #$59
CLC
ADC $02F0
STA $0220
BCC noC3	; Save C flag
LDA #$01
STA $0221
noC3:

BRK
