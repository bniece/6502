* = 0
data = $40

SEC
LDA $40
SBC $41 
STA $42
BRK

.dsb (data - *), $0
.byt $77,$39
