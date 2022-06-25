if keyb<>255 then
begin
  if isHelp then
  begin
    isHelp:=False;
    dpoke(DLIST_ADDR+25,SCREEN_WORK);
    keyb:=255;
  end;
  case keyb of
    k_esc: break;
    k_h: showHelp;
    k_return: choiceListFile;
    k_space:
      begin
        drawListSelection;
        gotoNEntry(curFile);
        getEntrySets(p_bank);
        p_bank:=p_bank xor $80;
        setEntrySets(p_bank);
        showList;
        drawListSelection;
      end;
    k_up,k_down:
      begin
        drawListSelection;
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
          if YFile<15 then
            inc(YFile)
          else
            if shFile<totalFiles-YFile-1 then v:=1;
        end;
        if v<>0 then
        begin
          inc(shFile,v);
          showList;
        end;
        curFile:=shFile+YFile;
        if curFile>=totalFiles then curFile:=totalFiles-1;
        drawListSelection;
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
