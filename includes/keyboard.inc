procedure toggleFile; Keep;
var tmp:word;

begin
  if isBitSet(screenStatus,ss_minMode) then exit;
  IOResult:=0;
  list_go2Entry(lstCurrent);
  tmp:=word(listPtr);
  list_getByte(p_type);
  list_getText(outStr);
  if (p_type and $3f=(fl_midifile and $3f)) then
  begin
    if IOResult<>ERR_LIST_ENTRY_END then
      list_getText(outStr);
    tglBit(p_type,$40);
    listPtr:=pointer(tmp);
    list_putByte(p_type);
    scradr:=listScrAdr[lstY];
    showEntry;
  end;
  IOResult:=0;
end;

procedure moveFileSelection; Keep;
var
  tmp:SmallInt absolute $88;

begin
  if isBitSet(screenStatus,ss_minMode) then exit;
  v:=0;
  if (keyb=k_UP) then
  begin
    if isBitSet(keymod,kmod_shift) then
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
    if isBitSet(keymod,kmod_shift) then
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
    if tmp<>0 then setBit(screenStatus,ss_isRefresh);
  end;
  lstCurrent:=lstShift+lstY;
end;

procedure fileAction; Keep;
begin
  lstCurrent:=lstShift+lstY;
  if stateInputLine=ils_pending then choiceListFile;
  if stateInputLine<>ils_done then exit;

  if p_type=fl_otherfile then
    IOResult:=ERR_NOT_A_MIDI_FILE;

  if isBitClr(screenStatus,ss_minMode) then
  begin
    stateInputLine:=ils_pending;
    if p_type=fl_playlist then
      IOResult:=loadPlaylist;
    if (p_type=fl_device) or
       (p_type=fl_directory) or
       (p_type=fl_parentDir) then
    begin
      clrBit(screenStatus,ss_plsMode);
      if resultInputLine then IOResult:=getDirectory;
    end;
    if IOResult and %11111100<>0 then statusError(IOResult);
  end;

  if p_type=fl_midifile then statusPlaying;
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
  tglBit(screenStatus,ss_view);
end;

procedure toggleMinMode; Keep;
begin
  tglBit(screenStatus,ss_minMode);
  if isBitClr(screenStatus,ss_minMode) then
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
  tglBit(screenStatus,ss_colorSet);
  setColors;
end;

procedure toggleHelpScreen;
var
  oLstY:byte;

begin
  tglBit(screenStatus,ss_isHelp);
  if isBitClr(screenStatus,ss_isHelp) then
  begin
    dpoke(DLIST_ADDR+24,SCREEN_WORK);
    lstY:=oLstY;
  end
  else
  begin
    dpoke(DLIST_ADDR+24,HELPSCR_ADDR);
    oLstY:=lstY;
    lstY:=255;
  end;
  keyb:=255;
end;

procedure playerControl; Keep;
begin
  if (keyb=k_Z) or (keyb=k_B) then // previous & next song
  begin
    setBit(stateInputLine,ils_noProcess);
    lstCurrent:=curPlay;
    repeat
      if (playerStatus and ps_loop=ps_shuffle) then
        lstCurrent:=random(lstTotal)
      else
      begin
        if keyb=k_B then v:=1 else v:=-1;
        inc(lstCurrent,v);
        if (lstCurrent>=lstTotal) then lstCurrent:=0;
        if (lstCurrent<0) then lstCurrent:=lstTotal;
      end;
      IOResult:=0;
      choiceListFile;
    until (p_type=fl_midifile) and (IOResult and %11111100=0);
    clrBit(stateInputLine,ils_noProcess);
    clearStatus;
    clearControls;
    if IOResult and %11111100<>0 then statusError(IOResult);
    if totalTracks<>0 then statusPlaying;
  end;

  if keyb=k_X then // Play
  begin
    statusStopped;
    statusPlaying;
  end;

  if (keyb=k_C) and isBitClr(playerStatus,ps_isStopped) then // Pause
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
