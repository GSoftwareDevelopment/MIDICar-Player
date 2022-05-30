procedure statusPlaying;
begin
  scradr:=SCREEN_STATUS;
  if totalTracks>0 then
  begin
    PutText(' Playing ');
    if isStopped then
    begin
      initTimer;
      isStopped:=false;
    end
    else
      _timerStatus:=_timerStatus and not f_counter;
  end
  else
  begin
    PutText(' No song ');
  end;
end;

procedure statusPause;
begin
  if isStopped then exit;
  scradr:=SCREEN_STATUS;
  PutText('  Pause  ');
  _timerStatus:=_timerStatus or f_counter;
end;

procedure statusStopped;
begin
  isStopped:=true;
  scradr:=SCREEN_STATUS;
  PutText(' Stopped ');
  _timerStatus:=_timerStatus or f_counter;
  _totalTicks:=0; _subCnt:=1;
  setIntVec(iTim1,oldTimerVec);
  Reset_MIDI;
  move(pointer(START_INFO_ADDR),pointer(TRACK_DATA_ADDR),512);
  chn:=0;
  clearUVMeters;
end;

procedure statusLoading;
begin
  scradr:=SCREEN_STATUS+2;
  PutText('Loading');
end;
