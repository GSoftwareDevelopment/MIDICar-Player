{

}

procedure readB; Inline; Assembler;
asm
event = $ff;

  ldy #0
  lda (_PTR),y
  sta event

  inc _adr
  bne skipMemBoundCheck
  inc _adr+1
  jsr memBoundCheck+5
// restore A reg
  lda event
skipMemBoundCheck:
end;

procedure readBpush; InLine; Assembler;
asm
event = $ff;

  ldy #0
  lda (_PTR),y
  jsr $2006

  inc _adr
  bne skipMemBoundCheck
  inc _adr+1
  jsr memBoundCheck+5
// restore A reg
  lda event
skipMemBoundCheck:
end;

procedure readVarL; Assembler;
asm
  icl 'midfile/asms/readvar.asm'
end;

procedure read24; Assembler;
asm
  icl 'midfile/asms/read24bigendian.a65'
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

    asm
      ldy #0
      lda (_ptr),y
      bpl status_run

    // zapisz pobrany bajt jako ostatnie zdarzenie (Status Run)
      sta _event

      inc _ADR
      bne skipMemBoundCheck
      inc _ADR+1
      jsr memBoundCheck
      lda _event
    skipMemBoundCheck:

      jmp eventCase

    status_run:
    // pobierz zdarzenie ze Status Run
      lda _event

    eventCase:
    // zapisz zdarzenie jako aktualne
      sta event

// event cases
      cmp #$C0
      bcs case_ev2
      case_ev3:
    end;
    eventMidi3;
    asm
      jmp case_endEvent

    case_ev2:
      cmp #$E0
      bcs case_ev4
    end;
    eventMidi2;
    asm
      jmp case_endEvent

    case_ev4:
      cmp #$F0
      bcc case_ev3

    case_evSysEx:
      cmp #$F8
      bcs case_evMeta
    end;
    eventSysEx;
    asm
      jmp case_endEvent

    case_evMeta:
      cmp #$ff
      bne case_endEvent
    end;
    eventMeta;
    asm
    case_endEvent:
    end;

  until _status and f_stop<>0;

  _status:=_status or f_skipDelta;
end;
