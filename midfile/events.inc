procedure eventMidi3; // Inline;
begin
{$IFDEF USE_FIFO}
  FIFO_WriteByte;
  readB; FIFO_WriteByte;
  readB; FIFO_WriteByte;
{$ELSE}
  MC6850_Send2;
  readB; MC6850_Send2;
  readB; MC6850_Send2;
{$ENDIF}
{
  The value read will represent the strength with which the sound will play.
}
  if (event>0) and (_event and $f0=$90) then
    chnVolume[_event and $0f]:=MC_Byte;
end;

procedure eventMidi2;
begin
{$IFDEF USE_FIFO}
  FIFO_WriteByte;
  readB; FIFO_WriteByte;
{$ELSE}
  MC6850_Send2;
  readB; MC6850_Send2;
{$ENDIF}
end;

procedure eventSysEx; // Inline;
begin
  readVarL;
{$IFDEF USE_FIFO}
  FIFO_WriteByte;
{$ELSE}
  MC6850_Send2;
{$ENDIF}
  while _tmp>0 do
  begin
    readB;
{$IFDEF USE_FIFO}
    FIFO_WriteByte;
{$ELSE}
    MC6850_Send2;
{$ENDIF}
    dec(_tmp);
  end;
// {$IFDEF USE_FIFO}
//         if FIFO_Byte=$F7 then flagSysEx:=false else flagSysEx:=true;
// {$ELSE}
//         if MC_Byte=$F7 then flagSysEx:=false else flagSysEx:=true;
// {$ENDIF}

end;

procedure eventMeta; // Inline;
begin
  readB; // fetch Meta event numer
  readVarL; // fetch data size

  if event=$2f then // end of track
  begin
    Dec(PlayingTracks);
    _status:=_status or f_stop;
  end
  else if event=$51 then // set tempo
  begin
    read24; // result in _delta!
    ms_per_qnote:=_delta;
    setTempo;
  end
  {$IFDEF USE_SUPPORT_VARS}
  else if event=$58 then // set tempo
  begin
    readB; tactNum:=event;
    readB; tactDenum:=event;
    readB; ticks_per_qnote:=event;
    readB; ticks_per_32nd:=event;
  end
  {$ENDIF}
  else
  // any orther meta event are skipped
    while _tmp>0 do
    begin
      dec(_tmp);
      inc(_adr);
      memBoundCheck;
    end;
end;
