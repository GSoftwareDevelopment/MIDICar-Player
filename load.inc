function loadSong(filename:PString):Byte;
const
  th:array[0..3] of byte = ($29,$1c,$1d,$1e);

var
  thi:byte;

  procedure loadPrc(v:byte);
  begin
    poke(SCREEN_STATUS,th[thi]);
    inc(thi); if thi=4 then thi:=0;
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
  clearStatus;
  statusLoading;
  loadProcess:=@loadPrc; thi:=0;
  result:=LoadMid(filename);
  clearStatus;
  if (result and %11111100)<>0 then
    totalTracks:=0
  else
    move(pointer(TRACK_DATA_ADDR),pointer(START_INFO_ADDR),512);
end;
