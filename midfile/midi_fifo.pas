unit MIDI_FIFO;

interface

var
  FIFO_Head:Byte absolute $fd;
  FIFO_Tail:Byte absolute $fe;
  FIFO_Byte:Byte absolute $ff;

procedure FIFO_Reset;
procedure FIFO_ReadByte; Inline;
procedure FIFO_PushDirect2MC6850; Inline;
procedure FIFO_WriteByte; Assembler; Inline;
procedure FIFO_Send(var data; len:byte);
procedure FIFO_Flush();

implementation
uses mc6850, SysUtils;

const
  FIFO_ADDR = $0600;

var
  FIFO_Buf:Array[0..255] of byte absolute FIFO_ADDR;

procedure FIFO_Reset;
begin
  FIFO_Head:=0;
  FIFO_Tail:=0;
  FillChar(@FIFO_Buf,256,0);
end;

procedure FIFO_PushDirect2MC6850; Inline;
begin
  if (MC6850_CNTRReg and TDRE)<>0 then
    if (FIFO_Tail<>FIFO_Head) then
    begin
      MC6850_BUFFER:=FIFO_Buf[FIFO_Tail];
      Inc(FIFO_Tail);
    end;
end;

procedure FIFO_ReadByte; Inline;
begin
  if (FIFO_Tail<>FIFO_Head) then
  begin
    FIFO_Byte:=FIFO_Buf[FIFO_Tail];
    Inc(FIFO_Tail);
  end;
end;

procedure FIFO_WriteByte; Assembler; Inline;
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
exitWrite:
end;

procedure FIFO_Send(var data; len:byte);
var
  p:^Byte;

begin
  p:=@data;
  while len>0 do
  begin
    FIFO_Byte:=p^;   FIFO_WriteByte;
    inc(p); dec(len);
  end;
end;

procedure FIFO_Flush;
begin
  While FIFO_Tail<>FIFO_Head do
  begin
    if (MC6850_CNTRReg and TDRE)<>0 then
    begin
      MC6850_BUFFER:=FIFO_Buf[FIFO_Tail];
      Inc(FIFO_Tail);
    end;
  end;
end;

end.