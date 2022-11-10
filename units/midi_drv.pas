unit MIDI_DRV;

interface

var
  FIFO_Head:Byte absolute $fd;
  FIFO_Tail:Byte absolute $fe;
  FIFO_Byte:Byte absolute $ff;

procedure drv_Reset;
procedure drv_WriteByte; Assembler; // Inline;
procedure drv_Flush;  Assembler; Keep;

implementation

procedure drv_Reset;
begin
  FIFO_Head:=0;
  FIFO_Tail:=0;
end;

procedure drv_WriteByte; Assembler; // Inline;
asm
// Check if the FIFO buffer is full?
  ldy FIFO_Head   // 3
  iny             // 2
  cpy FIFO_Tail   // 3
  bne storeInFIFO

// If it is full, flush it.
  jsr FIFO_Flush

storeInFIFO:
// Put a byte in the FIFO buffer
  ldy FIFO_Head // 3
  lda FIFO_Byte
  sta FIFO_ADDR,y
  inc FIFO_Head

// driver call `send` - try sending immediately
  jsr $2006

exitWrite:
end;

procedure drv_Flush; Assembler; Keep;
asm
XMTDON  = $3a

// driver call `send`
  jsr $2006

// wait until fifo not flush
loop:
	LDA XMTDON
	BEQ loop
end;

end.