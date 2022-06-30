if keyb<>255 then
begin
  if playerStatus and ps_isHelp<>0 then
  begin
    playerStatus:=playerStatus xor ps_isHelp;
    dpoke(DLIST_ADDR+25,SCREEN_WORK);
    keyb:=255;
  end;
  if stateInputLine=1 then
    do_inputLine
  else
    if keyb=k_ESC then break;

  case keyb of
    k_h:
      begin
        playerStatus:=playerStatus xor ps_isHelp;
        dpoke(DLIST_ADDR+25,HELPSCR_ADDR);
      end;
    k_return:
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
    k_space:
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
    k_up,k_down:
      begin
        if playerStatus and ps_isRefresh=0 then drawListSelection;
        v:=0;
        if (keyb=k_up) then
        begin
          if YFile>0 then
            dec(YFile)
          else
            if shFile>0 then v:=-1;
        end;
        if (keyb=k_down) then
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
    k_l:
      begin
        v:=(playerStatus+4) and ps_loop;
        playerStatus:=(playerStatus and (not ps_loop)) or v;
        statusLoop;
      end;
    k_m:
      begin
        clearUVMeters;
        playerStatus:=playerStatus xor ps_view;
      end;
    k_invers:
      begin
        playerStatus:=playerStatus xor ps_colorSet;
        setColors;
      end;
    k_z, k_b:
      begin
        if keyb=k_z then playDir:=255;
        if keyb=k_b then playDir:=1;
        StatusPause;
        playingTracks:=0;
      end;
    k_x:
      begin
        statusStopped;
        statusPlaying;
      end;
    k_c:
      begin
        if _timerStatus and f_counter<>0 then
          StatusPlaying
        else
          StatusPause;
      end;
    k_v: statusStopped;
    k_delete, k_clear, k_insert:
      begin
        if keyb=k_clear then inc(tempoShift,20000);
        if keyb=k_insert then dec(tempoShift,20000);
        if keyb=k_delete then tempoShift:=0;
        setTempo;
      end;
  end;
  keyb:=255;
end;
