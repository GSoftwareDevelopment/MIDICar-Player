procedure toggleFile; Keep;
begin
  if screenStatus and ss_minMode<>0 then exit;
  gotoNEntry(lstCurrent);
  getEntry(outStr);
  getEntrySets(p_bank);
  if (p_bank and $7f=(fl_midifile and $7f)) then
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
  if screenStatus and ss_minMode<>0 then exit;
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

procedure fileAction; Keep;
begin
  // if (screenStatus and ss_minMode<>0) then exit;
  lstCurrent:=lstShift+lstY;
  if stateInputLine=ils_pending then choiceListFile;
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

procedure toggleMinMode; Keep;
begin
  screenStatus:=screenStatus xor ss_minMode;
  if screenStatus and ss_minMode=0 then
    SDLST:=DLIST_ADDR
  else
    SDLST:=DLIST_MIN_ADDR;
end;

procedure toggleTimeShowMode; Keep;
begin
  inc(timeShowMode); if timeShowMode>2 then timeShowMode:=0;
  if timeShowMode=0 then
  begin
    scradr:=screen_time+54; putSpaces(6);
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

procedure playerControl; Keep;
begin
  if (keyb=k_Z) or (keyb=k_B) then // previous & next song
  begin
    stateInputLine:=stateInputLine xor 4;
    lstCurrent:=curPlay;
    repeat
      if keyb=k_B then v:=1 else v:=-1;
      asm
        lda playerStatus
        and ps_loop
        cmp #ps_shuffle
        bne sequenceLoop
      end;
        curPlay:=random(lstTotal);
      asm
        jmp endChoice
      sequenceLoop:
      end;
        inc(lstCurrent,v);
        if (lstCurrent>=lstTotal) then lstCurrent:=0;
        if (lstCurrent<0) then lstCurrent:=lstTotal;
      asm
      endChoice:
      end;
      choiceListFile;
    until (p_bank=fl_midifile) and (IOResult and %11111100=0);
    stateInputLine:=stateInputLine xor 4;
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
