LDA #$c0  ;Load the hex value $c0 into the A register
TAX       ;Transfer the value in the A register to X
INX       ;Increment the value in the X register

TXA		 ;Transfer the incremented value back to A
TAY		 ; and store it in Y

INY		 ;Increment Y
TYA		 ; and transfer it back to A

BRK       ;Break - we're done
