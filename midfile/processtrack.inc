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

procedure readVarL; Keep; Assembler;
asm
  icl 'midfile/asms/readvar.asm'
end;

procedure read24; Keep; Assembler;
asm
  icl 'midfile/asms/read24bigendian.a65'
end;

procedure ProcessTrack; Assembler;
asm
event = $FF ; yes, this is the same address as `FIFO_Byte` and `MC_Byte` :)

  ldy _bank \ lda $100,y \ sta PORTB

  jmp checkTrackProcess

; ------------------------
; begin track process loop

  loopTrackProcess:

  lda _status
  bmi skipDeltaTime

  jsr readVarL
  bcc continueTrackProcess

  lda _trackTime
  add _delta
  sta _trackTime
  lda _trackTime+1
  adc _delta+1
  sta _trackTime+1
  lda _trackTime+2
  adc _delta+2
  sta _trackTime+2
  lda _trackTime+3
  adc _delta+3
  sta _trackTime+3

  jmp endTrackProcess

skipDeltaTime:
  eor #f_skipDelta
  sta _status

continueTrackProcess:
  icl 'midfile/asms/get_event_byte.a65'
  icl 'midfile/asms/events_cases.a65'

checkTrackProcess:
  bit _status
  bvs endTrackProcess
  jmp loopTrackProcess

; end track process loop
; ----------------------

endTrackProcess:
  lda _status
  ora #f_skipDelta
  sta _status

end;
