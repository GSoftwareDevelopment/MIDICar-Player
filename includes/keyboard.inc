procedure toggleFile; Keep;
begin
  gotoNEntry(curFile);
  getEntry(outStr);
  getEntrySets(p_bank);
  if (p_bank<>fl_device) and (p_bank and $7f<>0) then
  begin
    drawListSelection;
    p_bank:=p_bank xor $80;
    setEntrySets(p_bank);
    showEntry;
    drawListSelection;
  end;
end;

procedure moveFileSelection; Keep;
begin
  if screenStatus and ss_isRefresh=0 then drawListSelection;
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
    screenStatus:=screenStatus or ss_isRefresh; // showList;
  end;
  curFile:=shFile+YFile;
  if curFile>=totalFiles then curFile:=totalFiles-1;
  if screenStatus and ss_isRefresh=0 then drawListSelection;
end;

procedure toggleLoopMode; Keep;
begin
  v:=(playerStatus+4) and ps_loop;
  playerStatus:=(playerStatus and (not ps_loop)) or v;
  statusLoop;
end;

procedure toggleMeters; Keep;
begin
  clearUVMeters;
  screenStatus:=screenStatus xor ss_view;
end;

procedure toggleTimeShowMode; Keep;
begin
  scradr:=screen_time+14; putSpaces(6);
  inc(timeShowMode); if timeShowMode>2 then timeShowMode:=0;
end;

procedure toggleScreenColors; Keep;
begin
  screenStatus:=screenStatus xor ss_colorSet;
  setColors;
end;

procedure toggleHelpScreen;
begin
  screenStatus:=screenStatus xor ss_isHelp;
  if screenStatus and ss_isHelp=0 then
    dpoke(DLIST_ADDR+25,SCREEN_WORK)
  else
    dpoke(DLIST_ADDR+25,HELPSCR_ADDR);
  keyb:=255;
end;

procedure playerControl; Keep;
begin
  if (keyb=k_Z) or (keyb=k_B) then // previous & next song
  begin
    if keyb=k_B then playDir:=1 else playDir:=255;
    v:=playerStatus and ps_loop;
    statusStopped;
    repeat
      if (v=ps_shuffle) then
        curPlay:=random(totalFiles)
      else
      begin
        inc(curPlay,playDir);
        if (curPlay=1) then curPlay:=totalFiles;
        if (curPlay=totalFiles) then begin curPlay:=1; break; end;
      end;
      curFile:=curPlay;
      IOResult:=1; choiceListFile; stateInputLine:=0;
    until (p_bank=fl_midifile) and (IOResult and %11111100=0);
    clearStatus;
    if IOResult and %11111100<>0 then statusError(IOResult);
    if totalTracks<>0 then statusPlaying;
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

procedure tempoControl; Keep;
begin
  if keyb=k_CLEAR then inc(tempoShift,20000);
  if keyb=k_INSERT then dec(tempoShift,20000);
  if keyb=k_DELETE then tempoShift:=0;
  setTempo;
end;

procedure fileAction; Keep;
begin
  choiceListFile;
  if stateInputLine<>ils_done then exit;

  stateInputLine:=ils_pending;
  if (p_bank=fl_device) or
      (p_bank=fl_directory) or
      (p_bank=fl_parentDir) then
  begin

    if resultInputLine then
    begin
      validPath;
      IOResult:=getDirectory;
      screenStatus:=screenStatus or ss_isRefresh;
    end;
  end;
  if IOResult and %11111100<>0 then statusError(IOResult);
  if p_bank=fl_midifile then statusPlaying;
end;
