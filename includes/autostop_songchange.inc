procedure AutoStopAndSongChange;
begin
  if (playerStatus and ps_isStopped=0) and (playingTracks=0) then
  begin
    v:=playerStatus and ps_loop;
    statusStopped;
    if v<>ps_playonce then
    begin
      if v>ps_repeatone then
      // if curPlay<>255 then
      begin
        repeat
          if (v=ps_shuffle) then
            curPlay:=random(totalFiles)
          else
          begin
            inc(curPlay,playDir);
            if (curPlay=1) then curPlay:=totalFiles;
            if (curPlay=totalFiles) then curPlay:=1;
          end;
          curFile:=curPlay;
          choiceListFile;
          if p_bank=$ff then
            IOResult:=loadSong(outStr)
          else
            continue;
        until IOResult<>1;
        clearStatus;
        if IOResult and %11111100<>0 then statusError(IOResult);
        playDir:=1;
      end;
      if totalTracks<>0 then statusPlaying;
    end;
  end;
end;
