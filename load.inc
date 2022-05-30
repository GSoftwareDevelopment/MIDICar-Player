procedure loadSong;
var
  err:byte;
  t:byte;

  procedure loadPrc(v:byte);
  begin
    poke(SCREEN_STATUS,$59+t);
    inc(t); if t=4 then t:=0;
    if v>0 then
    begin
      scradr:=SCREEN_STATUS+10;
      putInt(v-1); poke(scradr,$0f); inc(scradr);
      putInt(totalTracks); inc(scradr);
    end;
  end;

begin
  scradr:=screen_time; PutText(fn);
  statusLoading;
  loadProcess:=@loadPrc; t:=0;
  err:=LoadMid(fn);
  if err<>0 then
  begin
    scradr:=SCREEN_STATUS; puttextinvert:=128;
    if err<128 then
    case err of
      ERR_UNSUPPORTED_FORMAT: PutText(' Unsupported format ');
      ERR_NOT_ENOUGHT_MEMORY: PutText(' Not enought memory ');
    end
    else
    begin
      PutText(' I/O Error # '); putInt(err);
    end;
    while keyb=255 do ;
    puttextinvert:=0;
    totalTracks:=0; exit;
  end;
  move(pointer(TRACK_DATA_ADDR),pointer(START_INFO_ADDR),512);

  // statusCalculating;
  // _totalTicks:=100000000; FIFO2Null:=true;
  // cTrk:=totalTracks; playingTracks:=cTrk;
  // repeat
  //   loadPrc(0);
  //   processMIDI;
  // until playingTracks=0;
  // FIFO2Null:=false;
end;
