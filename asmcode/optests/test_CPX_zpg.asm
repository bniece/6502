;test_CPX_zpg
;Expected outcome:
;	A N V Z C
;	0200: 2A 00 00 01 01
;	0210: 2B 00 00 00 01
;	0220: 29 01 00 00 00
LDY #$01	; for marking set flags

LDA #$2A
STA $00

LDX #$2A
CPX $00	; CPX acts as if preceded by SEC
STX $0200
BPL pos1	; Save N flag
STY $0201
pos1:
BVC noV1	; Save V flag, shouldn't ever be set
STY $0202
noV1:
BNE zero1	; Save Z flag
STY $0203
zero1: 
BCC noC1	; Save C flag
STY $0204
noC1:

LDX #$2B
CPX $00
STX $0210
BPL pos2	; Save N flag
STY $0211
pos2:
BVC noV2	; Save V flag, shouldn't ever be set
STY $0212
noV2:
BNE zero2 ; Save Z flag
STY $0213
zero2:
BCC noC2	; Save C flag
STY $0214
noC2:

LDX #$29
CPX $00
STX $0220
BPL pos3	; Save N flag
STY $0221
pos3:
BVC noV3	; Save V flag, shouldn't ever be set
STY $0222
noV3:
BNE zero3	; Save Z flag
STY $0223
zero3:
BCC noC3	; Save C flag
STY $0224
noC3:

BRK
