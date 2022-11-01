procedure AutoStopAndSongChange;
begin
  if (playerStatus and ps_isStopped=0) and (playingTracks=0) then
  begin
    statusStopped;
    v:=playerStatus and ps_loop;
    if (v=ps_playOnce) then exit;
    if (v=ps_repeatOne) then
    begin
      statusPlaying; exit;
    end;
    keyb:=k_B;
  end;
end;
