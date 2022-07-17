unit MIDI_FIFO;

interface

var
  FIFO_Head:Byte absolute $fd;
  FIFO_Tail:Byte absolute $fe;
  FIFO_Byte:Byte absolute $ff;
  FIFO2Null:Boolean = False;

procedure FIFO_Reset;
procedure FIFO_PushDirect2MC6850; Assembler; // Inline;
procedure FIFO_WriteByte; Assembler; // Inline;
procedure FIFO_Flush;  Assembler; Keep;
// procedure FIFO_ReadByte; // Inline;
// procedure FIFO_Send(var data; len:byte);

implementation
uses mc6850;

const
  FIFO_ADDR = $0600;

var
  FIFO_Buf:Array[0..255] of byte absolute FIFO_ADDR;
  _timerStatus:Byte absolute $f5;

procedure FIFO_Reset;
begin
  FIFO_Head:=0;
  FIFO_Tail:=0;
end;

procedure FIFO_PushDirect2MC6850; Assembler; // Inline;
asm
  ldy FIFO_Tail
  cpy FIFO_Head
  beq exitPush

  lda MCBaseState:$d500 // MC6850.MC6850_CNTRREG
  and #MC6850.TDRE
  beq exitPush

  lda FIFO_ADDR,y
  sta MCBaseBuf:$d500  //MC6850.MC6850_BUFFER
  inc FIFO_Tail

exitPush:
end;

procedure FIFO_WriteByte; Assembler; // Inline;
asm
  lda FIFO_Head
  clc
  adc #1
  cmp FIFO_Tail
  bne storeInFIFO

  jsr FIFO_Flush

storeInFIFO:
  ldy FIFO_Head
  lda FIFO_Byte
  sta FIFO_ADDR,y
  inc FIFO_Head

  jsr FIFO_PushDirect2MC6850
exitWrite:
end;

procedure FIFO_Flush; Assembler; Keep;
asm
  sei
  // lda _timerStatus
  // eor #$80
  // sta _timerStatus

  ldy FIFO_Tail
flushLoop:
  cpy FIFO_Head
  beq endFlush

waitOnMC:
  lda MCBaseState:$d500 // MC6850.MC6850_CNTRReg
  and #MC6850.TDRE
  beq waitOnMc

  lda FIFO_ADDR,y
  sta MCBaseBuf:$d500   // MC6850.MC6850_BUFFER

  iny
  jmp flushLoop

endFlush:
  sty FIFO_Tail

  cli
  // lda _timerStatus
  // eor #$80
  // sta _timerStatus
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