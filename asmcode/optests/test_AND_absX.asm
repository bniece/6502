;test_AND_absX
;Expected outcome:
;	A N Z
;	0200: 2A 00 00
;	0210: 00 00 01
;	0220: 00 00 01
LDY #$01	; for marking flags
LDX #$00

LDA #$2A
STA $02FD
LDA #$80
STA $02FE
LDA #$00
STA $02FF

LDA #$2A
AND $02FD,X
STA $0200
BPL pos1	; Save N flag
STY $0201
pos1:
BNE zero1	; Save Z flag
STY $0202
zero1: 

INX
LDA #$2A
AND $02FD,X
STA $0210
BPL pos2	; Save N flag
STY $0211
pos2:
BNE zero2	; Save Z flag
STY $0212
zero2: 

INX
LDA #$2A
AND $02FD,X
STA $0220
BPL pos3	; Save N flag
STY $0221
pos3:
BNE zero3	; Save Z flag
STY $0222
zero3: 

BRK
