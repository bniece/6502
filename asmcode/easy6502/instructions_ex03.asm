LDA #$c0  ;Load the hex value $c0 into the A register
SEC		 ;Set carry for subtraction
SBC #$10  ;Subtract the hex value $10 from the A register
BRK       ;Break - we're done
