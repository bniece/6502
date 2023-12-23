;test_LSR_zpg
;Expected outcome:
;	A N Z C
;	00: 15 00 00 00
;	10: 00 00 01 01
;	20: 01 00 00 00
LDX #$00 ; zero page tends to be cluttered.  Clear it out
LDA #$00
loop:
STA $00,X
INX
CPX #$2F
BNE loop

LDY #$01	; for marking flags

LDA #$2A
STA $00
LDA #$01
STA $10
LDA #$02
STA $20

LSR $00
BPL pos1	; Save N flag
STY $01
pos1:
BNE zero1	; Save Z flag
STY $02
zero1: 
BCC carry1	; Save C flag
STY $03
carry1:

LSR $10
BPL pos2	; Save N flag
STY $11
pos2:
BNE zero2	; Save Z flag
STY $12
zero2: 
BCC carry2	; Save C flag
STY $13
carry2:

LSR $20
BPL pos3	; Save N flag
STY $21
pos3:
BNE zero3	; Save Z flag
STY $22
zero3: 
BCC carry3	; Save C flag
STY $23
carry3:

BRK
