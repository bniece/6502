;test_ASL_zpgX
;Expected outcome:
;	A A A 
;	N N N
;	Z Z z
;	C C C
;	00: 54 00 80
;	10: 00 00 01
;	20: 00 01 00
;	30: 00 01 00
LDX #$00 ; zero page tends to be cluttered.  Clear it out
LDA #$00
loop:
STA $00,X
INX
CPX #$2F
BNE loop

LDY #$01	; for marking flags
LDX #$00

LDA #$2A
STA $00
LDA #$80
STA $01
LDA #$40
STA $02

ASL $00,X
BPL pos1	; Save N flag
STY $10
pos1:
BNE zero1	; Save Z flag
STY $20
zero1: 
BCC carry1	; Save C flag
STY $30
carry1:
INX

ASL $00,X
BPL pos2	; Save N flag
STY $11
pos2:
BNE zero2	; Save Z flag
STY $21
zero2: 
BCC carry2	; Save C flag
STY $31
carry2:
INX

ASL $00,X
BPL pos3	; Save N flag
STY $12
pos3:
BNE zero3	; Save Z flag
STY $22
zero3: 
BCC carry3	; Save C flag
STY $32
carry3:

BRK
