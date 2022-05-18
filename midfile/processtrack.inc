{

}

procedure ProcessTrack;
// var
//   flagSysEx:Boolean;
//   msgLen:Byte;

  procedure readB; Inline;
  begin
    _tmp:=_ptr^;
    inc(_adr);
    memBoundCheck;
  end;

  procedure readB2FB; Inline;
  begin
{$IFDEF USE_FIFO}
    FIFO_Byte:=_ptr^;
{$ELSE}
    MC_Byte:=_ptr^;
{$ENDIF}
    inc(_adr);
    memBoundCheck;
  end;

{$i readVarL.inc}
{$i read24bits.inc}
{$i events.inc}

begin
  _timerStatus:=_timerStatus xor f_counter;     // pause ticking
  dTm:=_totalTicks-_trackTime;                 // time correction

  asm ldy _bank \ lda $100,y \ sta PORTB end;
  _delta:=0;
  repeat
    if (_status and f_skipDelta)=0 then
    begin
      readVarL;
      if _delta>0 then
      begin
        _trackTime:=_totalTicks+_delta-dTm;      // calculate new track time with time correction
        break;
      end;
    end
    else
      _status:=_status and not f_skipDelta;

    if _ptr^ and $80<>0 then
    begin
      ReadB; _event:=_tmp;
    end;

    if _event<=$bf then
      eventMidi3
    else if _event<=$df then
      eventMidi2
    else if _event<=$ef then
      eventMidi3
    else if _event<=$f7 then
      eventSysEx
    else if _event=$ff then
      eventMeta;

  until _status and f_stop<>0;

  _status:=_status or f_skipDelta;
  _timerStatus:=_timerStatus xor f_counter;  // resume ticking
end;
