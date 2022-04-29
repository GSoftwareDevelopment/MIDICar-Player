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
  deltaTime,totalTime:Longint;
  cTrk,PlayingTracks:Byte;
{$IFDEF DEBUG}
  QLen:byte;
{$ENDIF}

begin
  FIFO_Reset;
  MC6850_Reset;
  MC6850_Init(CD_64+WS_OddParity+WS_8bits);

  if paramCount=1 then
    fn:=ParamStr(1)
  else
    fn:='D2:OVERWORL.MID';

  MIDData:=Pointer($4000);
  MIDTracks:=Pointer($3F00);
  if not LoadMID(fn) then halt(1);
  totalTime:=0;

  Repeat
    PlayingTracks:=nTracks;
    for cTrk:=0 to nTracks-1 do
    begin
      Track:=@MIDTracks[cTrk*sizeOf(TMIDTrack)];
      if Track^.EOT then
      begin
        Dec(PlayingTracks); continue;
      end;
      deltaTime:=Track^.DeltaTime;
      if deltaTime=0 then
        deltaTime:=GetTrackData(Track);

      if deltaTime>0 then
      begin
        dec(deltaTime);
        Track^.DeltaTime:=deltaTime;
      end;
{$IFDEF DEBUG}
      if FIFO_Head<>FIFO_Tail then
      begin
{$IFDEF FIFO_DEBUG}
        WriteLn;
{$ENDIF}
        WriteLn(totalTime,' #',cTrk,' Head: ',FIFO_Head,' Tail: ',FIFO_Tail,' P:',Track^.DeltaTime);
      end;
{$ENDIF}
    end;
    inc(totalTime);
    FIFO_Flush;
  until PlayingTracks=0;
//  MIDI_SendNoteOn(0,64,64);
//  FIFO_Flush;
end.