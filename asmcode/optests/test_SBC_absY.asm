;test_SBC_absY
;Expected outcome:
;	A N V Z C
;	0200: 29 00 00 00 01
;	0210: EC 01 00 00 00
;	0220: 80 01 01 00 00
;	0230: 00 00 00 01 01
LDA #$2A
STA $02F0
LDA #$67
STA $02F1
LDA #$D3
STA $02F2
LDA #$53
STA $02F3

LDY #$00

LDA #$53
SEC
SBC $02F0,Y
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

LDA #$53
SEC
SBC $02F0,Y
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

LDA #$53
SEC
SBC $02F0,Y
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

INY

LDA #$53
SEC
SBC $02F0,Y
STA $0230
BPL pos4	; Save N flag
LDA #$01
STA $0231
pos4:
BVC noV4	; Save V flag
LDA #$01
STA $0232
noV4:
LDA $0230
BNE zero4	; Save Z flag
LDA #$01
STA $0233
zero4:
BCC noC4	; Save C flag
LDA #$01
STA $0234
noC4:

BRK
