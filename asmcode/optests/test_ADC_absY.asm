;test_ADC_absY
;Expected outcome:
;	A N V Z C
;	0200: 2B 00 00 00 00
;	0210: 80 01 01 00 00
;	0220: 00 00 00 01 01
LDA #$01
STA $02F0
LDA #$56
STA $02F1
LDA #$D6
STA $02F2

LDY #$00

LDA #$2A
CLC
ADC $02F0,Y
STA $0200
BPL pos1	; Save N flag
LDA #$01
STA $0201
pos1:
BVC noV1	; Save V flag
LDA #$01
STA $0202
noV1:
LDA $0200
BNE zero1	; Save Z flag
LDA #$01
STA $0203
zero1:
BCC noC1	; Save C flag
LDA #$01
STA $0204
noC1:

INY

LDA #$2A
CLC
ADC $02F0,Y
STA $0210
BPL pos2	; Save N flag
LDA #$01
STA $0211
pos2:
BVC noV2	; Save V flag
LDA #$01
STA $0212
noV2:
LDA $0210
BNE zero2 ; Save Z flag
LDA #$01
STA $0213
zero2:
BCC noC2	; Save C flag
LDA #$01
STA $0214
noC2:

INY

LDA #$2A
CLC
ADC $02F0,Y
STA $0220
BPL pos3	; Save N flag
LDA #$01
STA $0221
pos3:
BVC noV3	; Save V flag
LDA #$01
STA $0222
noV3:
LDA $0220
BNE zero3	; Save Z flag
LDA #$01
STA $0223
zero3:
BCC noC3	; Save C flag
LDA #$01
STA $0224
noC3:

BRK
