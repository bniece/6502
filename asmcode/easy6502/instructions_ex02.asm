LDA #$c0  ;Load the hex value $c0 into the A register
TAY       ;Transfer the value in the A register to Y
INY       ;Increment the value in the Y register
ADC #$c4  ;Add the hex value $c4 to the A register
BRK       ;Break - we're done
