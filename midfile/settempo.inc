procedure setTempo;
var
  freq:single;
  _freq:longint;
  fdiv:Byte;
  ratio:Single;
  _ratio:Byte;

begin
  _freq:=ms_per_qnote div tickDiv;
  freq:=_freq/1000000;
  freq:=1/freq;
  // freq:=1/((tickDiv*ticks_per_qnote)/1000000)*8;

  ratio:=250.9803/freq;
  _ratio:=1+trunc(ratio);
  if _ratio>15 then ratio:=15;

  // store timer ratio in status
  _timerStatus:=(_timerStatus and f_flags) or _ratio;

  // calc frequency divider for base timer
  fdiv:=round(64000/(freq*_ratio));

{$IFDEF USE_SUPPORT_VARS}
  // calc tempo (Beats Per Minutes)
  BPM:=60000000 div ms_per_qnote;
{$ENDIF}
  setIntVec(iTim1,@int_timer,0,fdiv);
end;
