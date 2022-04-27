Uses MC6850,MIDI_FIFO,MIDFiles;
{$I-}

Procedure MIDI_SendNoteOn(ch,note,vel:byte);
begin
  ZP_Data:=%10010000+ch and $0f;
  FIFO_WriteByte(ZP_Data);
  ZP_Data:=note and $7f;
  FIFO_WriteByte(ZP_Data);
  ZP_Data:=vel and $7f;
  FIFO_WriteByte(ZP_Data);
end;

var
  fn:string;
  Track:PMIDTrack;
  delta:Longint;

begin
  FIFO_Reset;
  MC6850_Reset;
  MC6850_Init(CD_64+WS_OddParity+WS_8bits);

  if paramCount=1 then
    fn:=ParamStr(1)
  else
    fn:='D2:SELFTEST.MID';

  MIDData:=Pointer($6000);
  MIDTracks:=Pointer($5F00);
  LoadMID(fn);

  Repeat
    Track:=@MIDTracks[0];
    delta:=Track^.DeltaTime;
    // WriteLn(delta);
    if delta=0 then
      GetTrackData(Track)
    else
    begin
      dec(delta);
      Track^.DeltaTime:=delta;
    end;
    FIFO_Flush;
  until Track^.EOT;
//  MIDI_SendNoteOn(0,64,64);
//  FIFO_Flush;
end.