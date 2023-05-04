procedure determineSongLength;
begin
  _songTicks:=0; curTrackPtr:=@MIDTracks;
  for cTrk:=1 to totalTracks do
  begin
    asm
    event = $FF

    ; copy track info to ZP registers

      ldy #sizeOf_TMIDTrack-1
    loop:
      lda (curTrackPtr),y
      sta _trkRegs,y
      dey
      bpl loop


      ldy _bank \ lda $100,y \ sta PORTB

    ; begin of determine current track length loop

    processTrack:

      jsr MAIN.MIDFILES.readVarL

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
      sta _tmp
      jmp endCase

    case_ev1:
      cmp #$F0
      bcs case_SysEx

    case_ev2:
      lda #2
      sta _tmp
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

      cmp #$01      ; meta for Text
      beq pushText
      cmp #$02      ; meta for Copyright
      beq pushText
      cmp #$03      ; meta for Sequence/Track name
      bne endCase

    pushText:

      ldx #1
    textCopyLoop:
      jsr MAIN.MIDFILES.readB
      sta MAIN.FILESTR.SNULL_ADDR,x
      dec _tmp
      beq sendPushText
      inx
      cpx #48
      bne textCopyLoop

    skipRestText:
      dec _tmp
      inc _ADR
      bne checkLen
      inc _ADR+1
      jsr memBoundCheck

    checkLen:
      lda _tmp
      bne skipRestText

    sendPushText:
      txa
      sta MAIN.FILESTR.SNULL_ADDR

      lda #<MAIN.FILESTR.SNULL_ADDR
      sta MAIN.MIDFILES.SENDPUSHLCDTEXT.STR
      lda #>MAIN.FILESTR.SNULL_ADDR
      sta MAIN.MIDFILES.SENDPUSHLCDTEXT.STR+1

      jsr sendPushLCDText

      jmp processTrack

    endCase:

    ; end of event cases
    ; ------------------

    ; skip event data
    ; length of data is in _tmp

    jmp checkDataLen
    readLoop:
      dec _tmp
      inc _ADR
      bne checkDataLen
      inc _ADR+1
      jsr memBoundCheck

    checkDataLen:
      lda _tmp
      bne readLoop

      jmp processTrack

    exitUntil:

    ; end of determine loop
    ; ---------------------

    end;

    if _trackTime>_songTicks then _songTicks:=_trackTime;
    inc(curTrackPtr,sizeOf(TMIDTrack));
  end;
end;