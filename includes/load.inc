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
  _bank:=totalXMS; _adr:=$4200; MIDFILES.fileSize:=0;

  updateSongTitle; // plugin

  clearStatus;
  putStatus(statusMsgs[4]);

  unsetNMI();

  loadProcess:=@loadPrc; thi:=0;
  result:=LoadMid;
  thi:=255;

  setNMI();

  if (result and %11111100)<>0 then
    totalTracks:=0
  else
  begin
    list_go2Entry(curPlay);
    list_getByte(_v);
    if _v=fl_curPlay then
    begin
      dec(listPtr);
      list_putByte(fl_midifile);
    end;
    curPlay:=lstCurrent;
    list_go2Entry(curPlay); list_putByte(fl_curPlay);
    asm
      lda PORTB
      pha
      ldy totalXMS
      lda $100,y
      sta PORTB
    end;
    move(pointer(TRACK_DATA_ADDR),pointer(START_INFO_ADDR),$200);
    asm
      pla
      sta PORTB
    end;
  end;
  setBit(screenStatus,ss_isRefresh);
  clearStatus;
  clearControls;
end;
