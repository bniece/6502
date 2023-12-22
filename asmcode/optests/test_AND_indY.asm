;test_AND_indY
;Expected outcome:
;	A N Z
;	0200: 2A 00 00
;	0210: 00 00 01
;	0220: 00 00 01
LDX #$01	; for marking flags
LDY #$00

LDA #$2A
STA $02FD
LDA #$80
STA $02FE
LDA #$00
STA $02FF

LDA #$02
STA $01
STA $03
STA $05
LDA #$FD
STA $00
LDA #$FE
STA $02
LDA #$FF
STA $04


LDA #$2A
AND ($00),Y
STA $0200
BPL pos1	; Save N flag
STX $0201
pos1:
BNE zero1	; Save Z flag
STX $0202
zero1: 

INY
LDA #$2A
AND ($00),Y
STA $0210
BPL pos2	; Save N flag
STX $0211
pos2:
BNE zero2	; Save Z flag
STX $0212
zero2: 

INY
LDA #$2A
AND ($00),Y
STA $0220
BPL pos3	; Save N flag
STX $0221
pos3:
BNE zero3	; Save Z flag
STX $0222
zero3: 

BRK
