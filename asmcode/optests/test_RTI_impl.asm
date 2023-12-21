;test_RTI_impl
;Expected outcome:
;	A = 0x2B
;	Carry not set
* = $0600

LDA #$06		; Set up stack as if it there was an interrupt
PHA			;    very non-portable code
LDA #$0A
PHA
PHP

JMP ISR	 ; Do the "interrupt"
LDA #$2B
BRK

ISR:
LDA #$2A
SEC		; Should clear when SR is restored
RTI

BRK