if keyb<>255 then
begin
  case keyb of
    k_esc: break;
    k_return: choicePlaylistFile;
    k_up,k_down:
      begin
        drawPlaylistSelection;
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
          gotoNEntry(shFile);
          showPlaylist;
        end;
        curFile:=shFile+YFile;
        if curFile>=totalFiles then curFile:=totalFiles-1;
        // scradr:=SCREEN_WORK+8; putINT(curFile);
        // poke(scradr,$0f); inc(scradr); putINT(totalFiles-1);
        // poke(scradr,$41);
        drawPlaylistSelection;
      end;
    k_l:
      begin
        playerStatus:=playerStatus xor ps_loop;
        isLooped;
      end;
    k_m:
      begin
        clearUVMeters;
        playerStatus:=playerStatus xor ps_view;
        // if playerStatus and ps_view=0 then
      end;
    k_invers:
      begin
        playerStatus:=playerStatus xor ps_colorSet;
        setColors;
      end;
    k_p:
      begin
        if _timerStatus and f_counter<>0 then
          StatusPlaying
        else
          StatusPause;
      end;
    k_s: statusStopped;
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
