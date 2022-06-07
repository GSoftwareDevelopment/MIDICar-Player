procedure loadSong;
var
  err:byte;
  t:byte;

  procedure loadPrc(v:byte);
  begin
    poke(SCREEN_STATUS,$5c+t);
    inc(t); if t=4 then t:=0;
    if v>0 then
    begin
      scradr:=SCREEN_STATUS+11;
      putInt(v-1); poke(scradr,$8f); inc(scradr);
      putInt(totalTracks);
      poke(scradr,$80);
    end;
  end;

begin
  // scradr:=screen_time; PutASCText(fn);
  clearStatus;
  statusLoading; puttextinvert:=128;
  loadProcess:=@loadPrc; t:=0;
  err:=LoadMid(fn);
  if err<>0 then
  begin
    scradr:=SCREEN_STATUS;
    if err<128 then
    case err of
      ERR_UNSUPPORTED_FORMAT: PutINTText(' Unsupported format '~*);
      ERR_NOT_ENOUGHT_MEMORY: PutINTText(' Not enought memory '~*);
    end
    else
    begin
      PutINTText(' I/O Error #'*~); putInt(err); poke(scradr,$80);
    end;
    while keyb=255 do ;
    puttextinvert:=0;
    totalTracks:=0; exit;
  end;
  move(pointer(TRACK_DATA_ADDR),pointer(START_INFO_ADDR),512);
  puttextinvert:=0;

  // statusCalculating;
  // _totalTicks:=100000000; FIFO2Null:=true;
  // cTrk:=totalTracks; playingTracks:=cTrk;
  // repeat
  //   loadPrc(0);
  //   processMIDI;
  // until playingTracks=0;
  // FIFO2Null:=false;
end;
