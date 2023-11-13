;test_LDA_indY
;Expected outcome:
;	A = 0x2C
;	0x10 = 0x2A
;	0x11 = 0x2B
;	0x12 = 0x2C
LDY #$01

LDX #$00
STX $01
LDX #$02
STX $02
LDX #$2A
STX $0201

LDA ($01),Y
STA $10

LDX #$FF
STX $03
LDX #$01
STX $04
LDX #$2B
STX $0200

LDA ($03),Y		; indirect address crosses page boundary
STA $11

LDX #$01
STX $FF
LDX #$02
STX $00
LDX #$2C
STX $0202

LDA ($FF),Y		; zero page address wraps back to begining
STA $12

BRK
