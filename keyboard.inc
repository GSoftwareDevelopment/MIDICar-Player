if keyb<>255 then
begin
  case keyb of
    k_esc: break;
    k_l:
      begin
        playerStatus:=playerStatus xor ps_loop;
        isLooped;
      end;
    k_m:
      begin
        playerStatus:=playerStatus xor ps_view;
        clearWorkArea;
        if playerStatus and ps_view<>0 then
          clearUVMeters;
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
    k_minus, k_plus, k_eq:
      begin
        if keyb=k_minus then inc(tempoShift,20000);
        if keyb=k_plus then dec(tempoShift,20000);
        if keyb=k_eq then tempoShift:=0;
        setTempo;
      end;
  end;
  keyb:=255;
end;
