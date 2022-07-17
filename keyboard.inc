procedure toggleFile;
begin
  gotoNEntry(curFile);
  getEntry(outStr);
  getEntrySets(p_bank);
  if (p_bank<>$fe) and (p_bank and $7f<>0) then
  begin
    drawListSelection;
    p_bank:=p_bank xor $80;
    setEntrySets(p_bank);
    showEntry;
    drawListSelection;
  end;
end;

procedure moveFileSelection;
begin
  if playerStatus and ps_isRefresh=0 then drawListSelection;
  v:=0;
  if (keyb=k_UP) then
  begin
    if YFile>0 then
      dec(YFile)
    else
      if shFile>0 then v:=-1;
  end;
  if (keyb=k_DOWN) then
  begin
    if YFile<LIST_VISIBLE_ITEMS-1 then
      inc(YFile)
    else
      if shFile<totalFiles-YFile-1 then v:=1;
  end;
  if v<>0 then
  begin
    inc(shFile,v);
    playerStatus:=playerStatus or ps_isRefresh;
    // showList;
  end;
  curFile:=shFile+YFile;
  if curFile>=totalFiles then curFile:=totalFiles-1;
  if playerStatus and ps_isRefresh=0 then drawListSelection;
end;

procedure toggleLoopMode;
begin
  v:=(playerStatus+4) and ps_loop;
  playerStatus:=(playerStatus and (not ps_loop)) or v;
  statusLoop;
end;

procedure toggleMeters;
begin
  clearUVMeters;
  playerStatus:=playerStatus xor ps_view;
end;

procedure toggleScreenColors;
begin
  playerStatus:=playerStatus xor ps_colorSet;
  setColors;
end;

procedure playerControl;
begin
  if (keyb=k_Z) or (keyb=k_B) then // previous & next song
  begin
    if keyb=k_Z then playDir:=255;
    if keyb=k_B then playDir:=1;
    StatusPause;
    playingTracks:=0;
  end;
  if keyb=k_X then // Play
    begin
      statusStopped;
      statusPlaying;
    end;
  if (keyb=k_C) and (playerStatus and ps_isStopped=0) then // Pause
  begin
    if _timerStatus and f_counter<>0 then
      StatusPlaying
    else
      StatusPause;
  end;
  if keyb=k_V then statusStopped;
end;

procedure tempoControl;
begin
  if keyb=k_CLEAR then inc(tempoShift,20000);
  if keyb=k_INSERT then dec(tempoShift,20000);
  if keyb=k_DELETE then tempoShift:=0;
  setTempo;
end;

procedure fileAction;
begin
  if choiceListFile then
  begin
  //   if p_bank=$fd then IOResult:=loadPlaylist(outStr);
    if p_bank=$fe then
    begin
      drawListSelection;
      init_inputLine;
    end;
    if p_bank=$ff then
    begin
      statusStopped;
      IOResult:=loadSong(outStr);
      stateInputLine:=2;
    end;
  end;
  if stateInputLine=2 then
  begin
    if (p_bank=$fe) then
    begin
      drawListSelection;
      if resultInputLine then
      begin
        fn:=outStr;
        if not getDeviceSpec(outstr,curDev) then joinStrings(curDev,fn);
        IOResult:=fileSelect(outStr);
        playerStatus:=playerStatus or ps_isRefresh;
      end;
    end;
    if IOResult and %11111100<>0 then statusError(IOResult);
    if p_bank=$ff then statusPlaying;
    stateInputLine:=0;
  end;
end;
