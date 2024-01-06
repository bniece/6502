* = $00
data = $40

LDX #0	; Index
LDA #$02	; STX for comparison to start of message
CHKSTX:
CMP $41,X
BEQ BEGCNT
INX
JMP CHKSTX
BEGCNT:
INX
STX $40	; Save starting position
LDA #$03	; ETX for comparison to end of message
CHKETX:
CMP $41,X
BEQ ENDCNT
INX
JMP CHKETX
ENDCNT:
TXA		; Move index to A for subtraction
SEC
SBC $40	; Subtract starting index
STA $40	; Save difference
BRK

.dsb (data - *), $0
.byte $00,$40,$02,$47,$4F,$03
;.byte $00,$40,$02,$47,"Yo.",$4F,$03
