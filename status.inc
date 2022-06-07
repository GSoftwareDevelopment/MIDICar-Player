procedure statusPlaying;
begin
  if totalTracks>0 then
  begin
    scradr:=SCREEN_TIME+1; putINTText(#$3b+#$00+#$00);
    scradr:=SCREEN_STATUS; PutINTText(' Playing '*~);
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
    scradr:=SCREEN_TIME+1; putINTText(#$00+#$00+#$00);
    scradr:=SCREEN_STATUS; PutINTText(' No song '*~);
  end;
end;

procedure statusPause;
begin
  if isStopped then exit;
  scradr:=SCREEN_TIME+1; putINTText(#$00+#$3c+#$00);
  scradr:=SCREEN_STATUS; PutINTText('  Pause  '*~);
  _timerStatus:=_timerStatus or f_counter;
end;

procedure statusStopped;
begin
  if totalTracks=0 then exit;
  if isStopped then exit;
  isStopped:=true;
  scradr:=SCREEN_TIME+1; putINTText(#$00+#$00+#$3d);
  scradr:=SCREEN_STATUS; PutINTText(' Stopped '*~);
  StopMIDI;
  move(pointer(START_INFO_ADDR),pointer(TRACK_DATA_ADDR),512);
  chn:=0;
  clearUVMeters;
end;

procedure statusLoading;
begin
  scradr:=SCREEN_STATUS+2; PutINTText(' Loading '*~);
end;
