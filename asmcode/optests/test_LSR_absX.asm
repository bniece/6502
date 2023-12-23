;test_LSR_abs
;Expected outcome:
;	A A A 
;	N N N
;	Z Z z
;	C C C
;	0200: 15 00 01
;	0210: 00 00 00
;	0220: 00 01 00
;	0230: 00 01 00
LDY #$01	; for marking flags
LDX #$00

LDA #$2A
STA $0200
LDA #$01
STA $0201
LDA #$02
STA $0202

LSR $0200,X
BPL pos1	; Save N flag
STY $0210
pos1:
BNE zero1	; Save Z flag
STY $0220
zero1: 
BCC carry1	; Save C flag
STY $0230
carry1:
INX

LSR $0200,X
BPL pos2	; Save N flag
STY $0211
pos2:
BNE zero2	; Save Z flag
STY $0221
zero2: 
BCC carry2	; Save C flag
STY $0231
carry2:
INX

LSR $0200,X
BPL pos3	; Save N flag
STY $0212
pos3:
BNE zero3	; Save Z flag
STY $0222
zero3: 
BCC carry3	; Save C flag
STY $0232
carry3:

BRK
