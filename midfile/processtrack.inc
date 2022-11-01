{

}

procedure readB; Inline;
var
  event:byte absolute $ff; // yes, this is the same address as `FIFO_Byte` and `MC_Byte` :)

begin
  event:=_ptr^;
  inc(_adr);
  memBoundCheck;
end;

procedure readVarL; Assembler;
asm
  icl 'midfile/asms/readvar.asm'
end;

procedure read24; Assembler;
asm
  icl 'midfile/asms/read24bigendian.pas'
end;

procedure ProcessTrack;
var
  event:byte absolute $ff; // yes, this is the same address as `FIFO_Byte` and `MC_Byte` :)

{$i events.inc}

begin
  asm ldy _bank \ lda $100,y \ sta PORTB end;
  repeat
    if (_status and f_skipDelta)=0 then
    begin
      readVarL;
      if _delta>0 then
      begin
        _trackTime:=_trackTime+_delta;
        break;
      end;
    end
    else
      _status:=_status and not f_skipDelta;

    if _ptr^ and $80<>0 then
    begin
      ReadB; _event:=event;
    end
    else
      event:=_event;

    if _event<$C0 then
      eventMidi3
    else if _event<$E0 then
      eventMidi2
    else if _event<$F0 then
      eventMidi3
    else if _event<$f8 then
      eventSysEx
    else if _event=$ff then
      eventMeta;

  until _status and f_stop<>0;

  _status:=_status or f_skipDelta;
end;
