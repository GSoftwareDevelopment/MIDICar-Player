unit MIDI_FIFO;

interface

var
  FIFO_Head:Byte absolute $fd;
  FIFO_Tail:Byte absolute $fe;
  FIFO_Byte:Byte absolute $ff;

procedure FIFO_Reset;
procedure FIFO_ReadByte;
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

procedure FIFO_ReadByte;
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
  rts

storeInFIFO:
  ldy FIFO_Head
  lda FIFO_Byte
  sta FIFO_ADDR,y
  iny
  sty FIFO_Head
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
    FIFO_Byte:=FIFO_Buf[FIFO_Tail];
{$IFDEF DEBUG}
    poke($d01a,FIFO_Byte);
{$ENDIF}
    MC6850_Send(FIFO_Byte);
    Inc(FIFO_Tail);
  end;
{$IFDEF DEBUG}
  poke($d01a,0);
{$ENDIF}
end;

end.