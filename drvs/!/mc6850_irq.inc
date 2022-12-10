var
  VIMIRQ:Pointer absolute $216;
  oldIRQ:Pointer;

procedure MC6850_IRQ; Assembler; Interrupt;
asm
FIFO_HEAD      equ $70
FIFO_TAIL      equ $71
MC6850_STATREG equ $d500
MC6850_BUFFER  equ $d501

  sta rega+1
  stx regx+1

  lda MC6850_STATREG  // check IRQ origin
  sta $d01a
  bpl skip            // N flag (7 bit) set means, IRQ is from MC6850, otherwise skip

  lda FIFO_TAIL       // check FIFO
  cmp FIFO_HEAD       // if nothing in
  beq skip            // skip

sendFromFIFO:

  ldx FIFO_TAIL       // get FIFO tail position
  lda $600,x          // get byte from FIFO
  sta MC6850_BUFFER   // send to MC6850
  inc FIFO_TAIL       // increment tail position

skip:
rega  lda #1
regx  ldx #1

  jmp (oldIRQ)
end;
