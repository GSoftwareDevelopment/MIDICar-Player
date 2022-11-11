procedure eventMidi3; // Inline;
begin
  asm
    // lda event
    jsr $2006
  end;
  readBPush;
  readBPush;
  // drv_WriteByte;
  // readB; drv_WriteByte;
  // readB; drv_WriteByte;
{
  The value read will represent the strength with which the sound will play.
}
  if (event>0) and (_event and $f0=$90) then
    chnVolume[_event and $0f]:=peek($ff); // MC_Byte;
end;

procedure eventMidi2;
begin
  asm
    // lda event
    jsr $2006
  end;
  readBPush;
  // drv_WriteByte;
  // readB; drv_WriteByte;
end;

procedure eventSysEx; // Inline;
begin
  asm
    // lda event
    jsr $2006
  end;
  readVarL;
  while _tmp>0 do
  begin
    readBPush;
    // readB;
    // drv_WriteByte;
    dec(_tmp);
  end;
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
