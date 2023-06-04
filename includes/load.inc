function loadSong:Byte;
var
  curLoad:Word absolute $D6;

  procedure loadPrc(v:byte);
  begin
    if v>0 then
    begin
      puttextinvert:=128;
      scradr:=SCREEN_STATUS+11;
      putInt(v-1); poke(scradr,$8f); inc(scradr);
      putInt(totalTracks);
      poke(scradr,$ce);
      puttextinvert:=0;
      dec(memAvailable,curLoad);
      updateMem;
    end;
  end;

begin
  statusStopped;
  validPath;
  if (_v and (dp_dev+dp_path)<>(dp_dev+dp_path)) then
  begin
    joinStrings(curDev,curPath);
    joinStrings(outStr,fn);
  end;

  calcAvailableMem;
  _bank:=totalXMS; _adr:=$4000;
  sendClearPushLCD;
  sendPushLCDText(fn);

  clearStatus;
  putStatus(statusMsgs[4]);
  loadProcess:=@loadPrc; thi:=0;
  result:=LoadMid;
  thi:=255;

  if (result and %11111100)<>0 then
    totalTracks:=0
  else
  begin
    curPlay:=lstCurrent;
    move(pointer(TRACK_DATA_ADDR),pointer(START_INFO_ADDR),512);
    if (playerStatus and ps_calcLen)<>0 then
    begin
      putStatus(statusMsgs[5]);
      determineSongLength;
      _songTicks:=_songTicks div 160;
    end;
  end;

  clearStatus;
end;
