procedure statusLoop;
const
  sloop:Array[0..3] of string[2] = (
    #$00+#$00,  // sloop_playone
    #$7e+#$7f,  // sloop_repeatone
    #$5c+#$5d,  // sloop_repeatall
    #$5e+#$5f   // sloop_shuffle
  );

begin
  scradr:=SCREEN_TIME+6;
  v:=(playerStatus and ps_loop) shr 2;
  putINTText(sloop[v]);
end;

procedure statusPlaying;
begin
  if totalTracks>0 then
  begin
    scradr:=SCREEN_TIME; putINTText(#$3b+#0+#$fc+#0+#$fd);
    scradr:=SCREEN_STATUS+1; PutINTText(#$4e+'Playing'*~+#$ce);
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
    scradr:=SCREEN_TIME; putINTText(#$fb+#0+#$fc+#0+#$fd);
    scradr:=SCREEN_STATUS+1; PutINTText(#$4e+'No song'*~+#$ce);
  end;
end;

procedure statusPause;
begin
  if isStopped then exit;
  scradr:=SCREEN_TIME; putINTText(#$fb+#0+#$3c+#0+#$fd);
  scradr:=SCREEN_STATUS+1; PutINTText(#$4e+' Pause '*~+#$ce);
  _timerStatus:=_timerStatus or f_counter;
end;

procedure statusStopped;
begin
  if totalTracks=0 then exit;
  if isStopped then exit;
  isStopped:=true;
  scradr:=SCREEN_TIME; putINTText(#$fb+#0+#$fc+#0+#$3d);
  scradr:=SCREEN_STATUS+1; PutINTText(#$4e+'Stopped'*~+#$ce);
  StopMIDI;
  move(pointer(START_INFO_ADDR),pointer(TRACK_DATA_ADDR),512);
  chn:=0;
  clearUVMeters;
end;

procedure statusLoading;
begin
  scradr:=SCREEN_STATUS+1; PutINTText(#$4e+'Loading '*~);
end;

procedure statusReadDirectory;
begin
  scradr:=SCREEN_STATUS+1; PutINTText(#$4e+'Reading directory '*~);
end;

// procedure statusReadPlaylist;
// begin
//   scradr:=SCREEN_STATUS+1; PutINTText(#$4e+'Reading playlist '*~);
// end;

procedure statusError(err:byte);
begin
  keyb:=255;
  scradr:=SCREEN_STATUS;
  case err of
    ERR_UNSUPPORTED_FORMAT: PutINTText(#$4e+'Unsupported format'~*+#$ce);
    ERR_NOT_ENOUGHT_MEMORY: PutINTText(#$4e+'Not enought memory'~*+#$ce);
    ERR_NOT_A_MIDI_FILE:    PutINTText(#$4e+'Not a MIDI file'~*+#$ce);
  else
    puttextinvert:=128;
    PutINTText(#$4e+'I/O Error #'*~); putInt(err); poke(scradr,$ce);
    puttextinvert:=0;
  end;
  while keyb=255 do ;
  clearStatus;
end;
