;test_EOR_zpgX
;Expected outcome:
;	A N Z
;	0200: 00 00 01
;	0210: AA 01 00
;	0220: 2A 00 00
LDY #$01	; for marking flags
LDX #$00

LDA #$2A
STA $00
LDA #$80
STA $01
LDA #$00
STA $02

LDA #$2A
EOR $00,X
STA $0200
BPL pos1	; Save N flag
STY $0201
pos1:
BNE zero1	; Save Z flag
STY $0202
zero1: 

INX
LDA #$2A
EOR $00,X
STA $0210
BPL pos2	; Save N flag
STY $0211
pos2:
BNE zero2	; Save Z flag
STY $0212
zero2: 

INX
LDA #$2A
EOR $00,X
STA $0220
BPL pos3	; Save N flag
STY $0221
pos3:
BNE zero3	; Save Z flag
STY $0222
zero3: 

BRK
