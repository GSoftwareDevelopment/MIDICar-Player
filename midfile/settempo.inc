procedure calculateCurrentTime;
begin
  timeInSec:=timeBeforeSetTempo+(_timerTick-ticksBeforeSetTempo)*ms_per_tick;
end;

function calcTempo:byte;
var
  freq:single;
  ratio:Single;
  _ratio:Byte;

begin
// Calculation of the master counter frequency
  _tickStep:=ticks_per_qnote div ticks_per_32nd;
  freq:=tickDiv/_tickStep;
  freq:=(us_per_qnote+tempoShift)/freq;
  freq:=1000000/freq;

// Calculation of the sub-counter divisor
{ NOTE:
The master counter cannot operate directly at a frequency below ~251Hz,
so it is multiplied by the value of the sub-counter divisor.
This yields apparently lower frequencies for the main counter.
}
  ratio:=250.6706/freq; // 250.6706 Smallest possible _frequency_ for TIMER1 with 8-bit resolution
  _ratio:=trunc(ratio);
  if _ratio>mf_ratio then _ratio:=mf_ratio;

  // store timer ratio in status
  _timerStatus:=(_timerStatus and mf_flags) or _ratio;
  _subCnt:=0;
  // calc frequency divider for timer
  result:=round(63921/(freq*(1+_ratio)));
end;

procedure calcTime;
begin
  ticksBeforeSetTempo:=_timerTick;
  timeBeforeSetTempo:=timeInSec;
{$IFDEF USE_SUPPORT_VARS}
  // calc tempo (Beats Per Minutes)
  BPM:=60000000/us_per_qnote;
  ms_per_tick:=(60000/(BPM*tickDiv))/1000;
  // we get the current second of the song (for the current tempo!)
  // when `totalSec:=round(_timerTick*ms_per_tick);`
{$ENDIF}
end;

procedure setTempo;
var
  fdiv:Byte;

begin
  _timerStatus:=_timerStatus xor $80;
  fdiv:=calcTempo;
  calcTime;
  asm
    sei  // disable IRQ
  // reset POKEY
    lda #$00
    sta AUDC1
  // setup TIMER1
    mva FDIV AUDF1
  // start timer strobe
    sta stimer

    cli  // enable IRQ
  end;
  _timerStatus:=_timerStatus xor $80;
end;
