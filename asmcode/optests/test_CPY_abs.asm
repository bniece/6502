;test_CPY_abs
;Expected outcome:
;	A N V Z C
;	0200: 2A 00 00 01 01
;	0210: 2B 00 00 00 01
;	0220: 29 01 00 00 00
LDX #$01	; for marking set flags

LDA #$2A
STA $02FF

LDY #$2A
CPY $02FF	; CPY acts as if preceded by SEC
STY $0200
BPL pos1	; Save N flag
STX $0201
pos1:
BVC noV1	; Save V flag, shouldn't ever be set
STX $0202
noV1:
BNE zero1	; Save Z flag
STX $0203
zero1: 
BCC noC1	; Save C flag
STX $0204
noC1:

LDY #$2B
CPY $02FF
STY $0210
BPL pos2	; Save N flag
STX $0211
pos2:
BVC noV2	; Save V flag, shouldn't ever be set
STX $0212
noV2:
BNE zero2 ; Save Z flag
STX $0213
zero2:
BCC noC2	; Save C flag
STX $0214
noC2:

LDY #$29
CPY $02FF
STY $0220
BPL pos3	; Save N flag
STX $0221
pos3:
BVC noV3	; Save V flag, shouldn't ever be set
STX $0222
noV3:
BNE zero3	; Save Z flag
STX $0223
zero3:
BCC noC3	; Save C flag
STX $0224
noC3:

BRK
