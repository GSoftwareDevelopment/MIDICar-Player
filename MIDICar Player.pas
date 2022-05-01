Uses
  MC6850,
{$IFDEF USE_FIFO}MIDI_FIFO,{$ENDIF}
  MIDFiles;
{$I-}

var
  fn:PString;
  Track:PMIDTrack;
  curTrackOfs:Byte;
  trackTime,dTm:Longint;
  cTrk,PlayingTracks:Byte;

begin
{$IFDEF USE_FIFO}
  FIFO_Reset;
{$ENDIF}
  MC6850_Reset;
  MC6850_Init(CD_64+WS_OddParity+WS_8bits);

  if paramCount=1 then
    fn:=ParamStr(1)
  else
    fn:='D2:SELFTEST.MID';

  MIDTracks:=Pointer($4000);
  // MIDData:=Pointer($4100);
  if not LoadMID(fn) then halt(1);
  totalTicks:=0;

  setTempo(500000); // 120 BPM

  Repeat
    PlayingTracks:=nTracks; curTrackOfs:=0;
    for cTrk:=1 to nTracks do
    begin
      Track:=@MIDTracks[curTrackOfs];
      inc(curTrackOfs,sizeOf(TMIDTrack));
      if not Track^.EOT then
      begin
        trackTime:=Track^.DeltaTime;
        if totalTicks>=trackTime then
        begin
          _pauseCount:=true;
          dTm:=totalTicks-trackTime;
          trackTime:=GetTrackData(Track);
          Track^.deltaTime:=totalTicks+trackTime-dTm;
          _pauseCount:=false;
        end;
      end
      else
      begin
        Dec(PlayingTracks); continue;
      end;
    end;

{$IFDEF USE_FIFO}
    _pauseCount:=true;
  // if FIFO_ReadByte(ZP_Data) then MC6850_Send(ZP_Data);
    FIFO_Flush;
    _pauseCount:=false;
{$ENDIF}

  until PlayingTracks=0;

  setIntVec(iTim1,oldVec);
end.