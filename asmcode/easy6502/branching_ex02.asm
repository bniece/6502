  LDA #$F9
  STA $00
  LDX #$00
loop:
  STX $01
  LDA $00
  ADC $01
  STA $10,X
  INX
  BCC loop

  BRK
