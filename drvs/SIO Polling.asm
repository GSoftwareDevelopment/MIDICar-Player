sei
lda #%00100000
bit IRQST
bne *-3
lda #%00000000
sta IRQEN
lda #%00100000
sta IRQEN
lda SERIN
cli