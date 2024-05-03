;test_ROR_abs
;Expected outcome:
;	A N Z C
;	0200: 15 00 00 00
;	0210: 00 00 01 01
;	0220: 81 01 00 00
CLC	; for a clean starting point

LDY #$01	; for marking flags

LDA #$2A
STA $0200
LDA #$01
STA $0210
LDA #$02
STA $0220

ROR $0200
BPL pos1	; Save N flag
STY $0201
pos1:
BNE zero1	; Save Z flag
STY $0202
zero1: 
BCC carry1	; Save C flag
STY $0203
carry1:

ROR $0210
BPL pos2	; Save N flag
STY $0211
pos2:
BNE zero2	; Save Z flag
STY $0212
zero2: 
BCC carry2	; Save C flag
STY $0213
carry2:

ROR $0220
BPL pos3	; Save N flag
STY $0221
pos3:
BNE zero3	; Save Z flag
STY $0222
zero3: 
BCC carry3	; Save C flag
STY $0223
carry3:

BRK
