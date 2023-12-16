* = $0600
  LDX #$00
  LDY #$00
start:
  STX $00,Y
  CPX #$05
  BEQ end
  INX
  INY
  JMP start
end:
  BRK
