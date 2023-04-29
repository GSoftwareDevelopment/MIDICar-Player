{

}

procedure readB; Keep; Assembler;
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

; ------------------------
; begin track process loop

  loopTrackProcess:

  bit _trackTime+3
  bvs skipDeltaTime

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

  lda _trackTime+3
  ora #f_skipDelta
  sta _trackTime+3

  rts

skipDeltaTime:
  lda _trackTime+3
  and #%00111111
  sta _trackTime+3

continueTrackProcess:
  icl 'midfile/asms/get_event_byte.a65'
  icl 'midfile/asms/events_cases.a65'

  jmp loopTrackProcess

; end track process loop
; ----------------------

endTrackProcess:

end;
