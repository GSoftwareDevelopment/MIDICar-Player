{
  Implementation based on code from the website
  https://blog.stratifylabs.dev/device/2013-10-02-A-FIFO-Buffer-Implementation/
}
unit MIDI_FIFO;

interface

var
  ZP_Data:Byte absolute $ff;
  FIFO_Head:Byte absolute $F0;
  FIFO_Tail:Byte absolute $F1;

procedure FIFO_Reset();
function FIFO_ReadByte(var data:Byte):boolean;
function FIFO_WriteByte(data:byte):boolean;
procedure FIFO_Flush();

implementation
uses mc6850, SysUtils;

const
  FIFO_SIZE = 255;
  FIFO_ADDR = $0600;

var
  FIFO_Buf:Array[0..FIFO_SIZE] of byte absolute FIFO_ADDR;

procedure FIFO_Reset;
begin
  FIFO_Head:=0;
  FIFO_Tail:=0;
  FillChar(@FIFO_Buf,FIFO_SIZE,0);
end;

function FIFO_ReadByte(var data:Byte):boolean;
begin
  if (FIFO_Tail<>FIFO_Head) then
  begin
    data:=FIFO_Buf[FIFO_Tail];
    Inc(FIFO_Tail);
    if (FIFO_Tail=FIFO_SIZE) then FIFO_TAIL:=0;
    result:=true;
  end
  else
    result:=false;
end;

function FIFO_WriteByte(data:byte):boolean;
begin
  if ((FIFO_Head+1)=FIFO_Tail) or
     (((FIFO_Head+1)=FIFO_SIZE) and (FIFO_Tail=0)) then
       exit(false) // no more room
  else
  begin
    FIFO_Buf[FIFO_Head]:=data;
{$IFDEF FIFO_DEBUG}
    Write(IntToHex(data,2));
{$ENDIF}
    inc(FIFO_Head);
    if (FIFO_Head=FIFO_Size) then FIFO_Head:=0;
  end;
  result:=true;
end;

procedure FIFO_Flush;
begin
  While FIFO_ReadByte(ZP_Data) do
    MC6850_Send(ZP_Data);
end;

end.