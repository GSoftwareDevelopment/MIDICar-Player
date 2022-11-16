function loadSong:Byte;
var
  curLoad:Word absolute $D6;

  procedure loadPrc(v:byte);
  begin
    if v>0 then
    begin
      puttextinvert:=128;
      scradr:=SCREEN_STATUS+10;
      putInt(v-1); poke(scradr,$8f); inc(scradr);
      putInt(totalTracks);
      poke(scradr,$ce);
      puttextinvert:=0;
      dec(memAvailable,curLoad);
      updateMem;
    end;
  end;

begin
  resetMemoryPtr;
  _bank:=last_bank; _adr:=last_adr;

  clearStatus;
  statusLoading;
  loadProcess:=@loadPrc; thi:=0;
  result:=LoadMid;
  thi:=255;
  clearStatus;
  if (result and %11111100)<>0 then
    totalTracks:=0
  else
  begin
    curPlay:=lstCurrent;
    move(pointer(TRACK_DATA_ADDR),pointer(START_INFO_ADDR),512);
    statusCalcLength;
    determineSongLength;
    _songTicks:=_songTicks div 160;
    clearStatus;
  end;
end;
