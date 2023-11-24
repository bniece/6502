;test_BVS_rel
;Expected outcome:
;	0x00 = 0x80
;	0x01 = 0x81
;	0x02 = 0x7F
;	0x03 = 0x7E
CLV
LDA #$7F
CLC
ADC #$01
BVS overflow1
JMP end
overflow1:
STA $00

CLV
LDA #$7F
SEC
SBC #$FE
BVS overflow2
JMP end
overflow2:
STA $01

CLV 
LDA #$80
CLC
ADC #$FF
BVS overflow3
JMP end
overflow3:
STA $02

CLV
LDA #$80
SEC
SBC #$02
BVS overflow4
JMP end
overflow4:
STA $03

end:
BRK
