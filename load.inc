function loadSong(filename:PString):Byte;

  procedure loadPrc(v:byte);
  begin
    // poke(SCREEN_STATUS,th[thi]);
    // inc(thi); if thi=4 then thi:=0;
    if v>0 then
    begin
      puttextinvert:=128;
      scradr:=SCREEN_STATUS+10;
      putInt(v-1); poke(scradr,$8f); inc(scradr);
      putInt(totalTracks);
      poke(scradr,$ce);
      puttextinvert:=0;
    end;
  end;

begin
  _bank:=last_bank; _adr:=last_adr;
  clearStatus;
  statusLoading;
  loadProcess:=@loadPrc; thi:=0;
  result:=LoadMid(filename);
  thi:=255;
  clearStatus;
  if (result and %11111100)<>0 then
  begin
    curPlay:=255; totalTracks:=0;
  end
  else
  begin
    move(pointer(TRACK_DATA_ADDR),pointer(START_INFO_ADDR),512);
    curPlay:=curFile;
  end;
end;
