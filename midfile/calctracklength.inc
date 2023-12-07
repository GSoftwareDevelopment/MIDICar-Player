procedure calcTrackLength;
begin
  resetVars;
  _eStatusRun:=0;
  asm
  event = $FF

    ldy MAIN.MIDFILES._bank \ lda $100,y \ sta PORTB

  ; begin of determine current track length loop

  processTrack:

    jsr MAIN.MIDFILES.readVarL

    bcc continueTrackProcess

      lda MAIN.MIDFILES._trackTick
      add MAIN.MIDFILES._delta
      sta MAIN.MIDFILES._trackTick
      lda MAIN.MIDFILES._trackTick+1
      adc MAIN.MIDFILES._delta+1
      sta MAIN.MIDFILES._trackTick+1
      lda MAIN.MIDFILES._trackTick+2
      adc MAIN.MIDFILES._delta+2
      sta MAIN.MIDFILES._trackTick+2
      lda MAIN.MIDFILES._trackTick+3
      adc MAIN.MIDFILES._delta+3
      sta MAIN.MIDFILES._trackTick+3

  continueTrackProcess:

    icl 'midfile/asms/get_event_byte.a65'

  ; --------------------
  ; start of event cases

  eventCase:
    cmp #$C0
    bcc case_ev2

    cmp #$E0
    bcs case_ev1

    lda #1
    sta MAIN.MIDFILES._tmp
    jmp endCase

  case_ev1:
    cmp #$F0
    bcs case_SysEx

  case_ev2:
    lda #2
    sta MAIN.MIDFILES._tmp
    jmp endCase

  case_SysEx:
    cmp #$F8
    bcs case_Meta

    jsr MAIN.MIDFILES.readVarL

  case_Meta:
    cmp #$FF      ; meta event signature
    bne endCase

    MAIN.MIDFILES.PROCESSTRACK.m@read "sta event" ""

    jsr MAIN.MIDFILES.readVarL

    lda event
    cmp #$2F      ; end of track
    beq exitUntil

  endCase:

  ; end of event cases
  ; ------------------

  ; skip event data
  ; length of data is in _tmp

    jmp checkDataLen
  readLoop:
    dec MAIN.MIDFILES._tmp
    inc MAIN.MIDFILES._ADR
    bne checkDataLen
    inc MAIN.MIDFILES._ADR+1
    jsr MAIN.MIDFILES.memBoundCheck

  checkDataLen:
    lda MAIN.MIDFILES._tmp
    bne readLoop

    jmp processTrack

  exitUntil:

  ; end of determine loop
  ; ---------------------

  end;
end;