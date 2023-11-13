;test_STA_indY
;Expected outcome:
;	0x0200 = 0x2B
;	0x0201 = 0x2A
;	0x0202 = 0x2C
LDY #$01

LDA #$00
STA $01
LDA #$02
STA $02

LDA #$2A
STA ($01),Y

LDA #$FF
STA $03
LDA #$01
STA $04

LDA #$2B
STA ($03),Y		; indirect address crosses page boundary

LDA #$01
STA $FF
LDA #$02
STA $00

LDA #$2C
STA ($FF),Y		; zero page address wraps back to begining

BRK
