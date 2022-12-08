procedure toggleFile; Keep;
begin
  gotoNEntry(lstCurrent);
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
var
  tmp:SmallInt absolute $88;

begin
  if screenStatus and ss_isRefresh=0 then drawListSelection;
  v:=0;
  if (keyb=k_UP) then
  begin
    if keymod and kmod_shift<>0 then
      if lstY>0 then
        lstY:=0                     // move to top of screen
      else
        v:=-LIST_VISIBLE_ITEMS      // scrool one page up
    else
      if lstY>0 then
        dec(lstY)                   // move one line up
      else
        v:=-1;                      // scroll one line up
  end;
  if (keyb=k_DOWN) then
  begin
    if keymod and kmod_shift<>0 then
      if lstY<LIST_VISIBLE_ITEMS-1 then
        lstY:=LIST_VISIBLE_ITEMS-1  // move to bottom of screen
      else
        v:=LIST_VISIBLE_ITEMS       // scroll one page down
    else
      if lstY<LIST_VISIBLE_ITEMS-1 then
        inc(lstY)                   // move one line down
      else
        v:=1;                       // scroll one line down
  end;
  lstCurrent:=lstShift+lstY+SmallInt(v);
  if v=0 then
  begin
    tmp:=lstTotal-1;
    if (lstCurrent<0) or (lstCurrent>tmp) then
    begin
      if keyb=k_up then
        v:=1
      else
        v:=tmp-lstCurrent;
      inc(lstY,v);
      v:=0;
    end;
  end
  else
  begin
    tmp:=lstShift;
    inc(lstShift,v);
    if (lstShift+LIST_VISIBLE_ITEMS)>lstTotal then lstShift:=lstTotal-LIST_VISIBLE_ITEMS;
    if lstShift<0 then lstShift:=0;
    tmp:=tmp-lstShift;
    if tmp<>0 then screenStatus:=screenStatus or ss_isRefresh;
  end;
  lstCurrent:=lstShift+lstY;
  if screenStatus and ss_isRefresh=0 then drawListSelection;
end;

procedure toggleLoopMode; Keep;
begin
  v:=(playerStatus+1) and ps_loop;
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
  inc(timeShowMode); if timeShowMode>2 then timeShowMode:=0;
  if timeShowMode=0 then
  begin
    scradr:=screen_time+14; putSpaces(6);
  end;
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

procedure fileAction; Keep;
begin
  choiceListFile;
  if stateInputLine<>ils_done then exit;

  stateInputLine:=ils_pending;
  if (p_bank=fl_device) or
      (p_bank=fl_directory) or
      (p_bank=fl_parentDir) then
  begin

    if resultInputLine then IOResult:=getDirectory;
  end;
  if IOResult and %11111100<>0 then statusError(IOResult);
  if p_bank=fl_midifile then statusPlaying;
end;

procedure playerControl; Keep;
var
  playMode:Byte;
begin
  if (keyb=k_Z) or (keyb=k_B) then // previous & next song
  begin
    if keyb=k_B then v:=1 else v:=-1;
    playMode:=playerStatus and ps_loop;
    statusStopped;
    repeat
      if (playMode=ps_shuffle) then
        curPlay:=random(lstTotal)
      else
      begin
        inc(curPlay,v);
        if (curPlay=1) then curPlay:=lstTotal;
        if (curPlay=lstTotal) then begin curPlay:=1; break; end;
      end;
      lstCurrent:=curPlay;
      IOResult:=1;
      choiceListFile;
      stateInputLine:=0;
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
