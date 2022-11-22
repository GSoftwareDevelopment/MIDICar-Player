unit MIDI_FIFO;

interface

var
  FIFO_Head:Byte absolute $fd;
  FIFO_Tail:Byte absolute $fe;
  FIFO_Byte:Byte absolute $ff;

procedure FIFO_Reset;
// procedure FIFO_PushDirect2MC6850; Assembler; // Inline;
procedure FIFO_WriteByte; Assembler; // Inline;
procedure FIFO_Flush;  Assembler; Keep;
// procedure FIFO_ReadByte; // Inline;
// procedure FIFO_Send(var data; len:byte);

implementation
// uses mc6850;

const
  FIFO_ADDR = $0600;

var
  FIFO_Buf:Array[0..255] of byte absolute FIFO_ADDR;
  // _timerStatus:Byte absolute $f5;

procedure FIFO_Reset;
begin
  FIFO_Head:=0;
  FIFO_Tail:=0;
end;

// procedure FIFO_PushDirect2MC6850; Assembler; // Inline;
// asm
// // check if FIFO buffer is empty?
//   ldy FIFO_Tail
//   cpy FIFO_Head
//   beq exitPush

// //
// // check MC buffer is clear
//   lda MCBaseState:$d500 // MC6850.MC6850_CNTRREG
//   and #MC6850.TDRE
//   beq exitPush

//   lda FIFO_ADDR,y
//   sta MCBaseBuf:$d500  //MC6850.MC6850_BUFFER
//   inc FIFO_Tail

//   sec
//   rts
// exitPush:
//   clc
// end;

procedure FIFO_WriteByte; Assembler; // Inline;
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

procedure FIFO_Flush; Assembler; Keep;
asm
XMTDON  = $3a

// driver call `send`
  jsr $2006

// wait until fifo not flush
loop:
	LDA XMTDON
	BEQ loop

//   sei

//   ldy FIFO_Tail
// flushLoop:
//   cpy FIFO_Head
//   beq endFlush

// waitOnMC:
//   lda MCBaseState:$d500 // MC6850.MC6850_CNTRReg
//   and #MC6850.TDRE
//   beq waitOnMc

//   lda FIFO_ADDR,y
//   sta MCBaseBuf:$d500   // MC6850.MC6850_BUFFER

//   iny
//   jmp flushLoop

// endFlush:
//   sty FIFO_Tail

//   cli
end;

{
procedure FIFO_ReadByte; // Inline;
beginzdarza się, że człowiek
  if (FIFO_Tail<>FIFO_Head) then
  begin
    FIFO_Byte:=FIFO_Buf[FIFO_Tail];
    Inc(FIFO_Tail);
  end;
end;

procedure FIFO_Send(var data; len:byte);
var
  p:^Byte;

begin
  p:=@data;
  while len>0 do
  begin
    FIFO_Byte:=p^; FIFO_WriteByte;
    inc(p); dec(len);
  end;
end;

// old FIFO_FLUSH
begin
  _timerStatus:=_timerStatus xor $80;
  While FIFO_Tail<>FIFO_Head do
  begin
    if (MC6850_CNTRReg and TDRE)<>0 then
    begin
      MC6850_BUFFER:=FIFO_Buf[FIFO_Tail];
      Inc(FIFO_Tail);
    end;
  end;
  _timerStatus:=_timerStatus xor $80;
end;
}

end.