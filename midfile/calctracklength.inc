procedure calcTrackLength; assembler;
asm
event = $FF

  ldy MAIN.MIDFILES._bank \ lda $100,y \ sta PORTB

; begin of determine current track length loop

processTrack:

  jsr MAIN.MIDFILES.readVarL

  bcc continueTrackProcess

    lda MAIN.MIDFILES._trackTime
    add MAIN.MIDFILES._delta
    sta MAIN.MIDFILES._trackTime
    lda MAIN.MIDFILES._trackTime+1
    adc MAIN.MIDFILES._delta+1
    sta MAIN.MIDFILES._trackTime+1
    lda MAIN.MIDFILES._trackTime+2
    adc MAIN.MIDFILES._delta+2
    sta MAIN.MIDFILES._trackTime+2
    lda MAIN.MIDFILES._trackTime+3
    adc MAIN.MIDFILES._delta+3
    sta MAIN.MIDFILES._trackTime+3

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

/*
TODO: Wykrycie meta zdarzenia typu: Text, Copyright lub Sequence/Track name
    dodaje na końcu danych MIDa wskaźnik:
    - bank
    - adres do tego tekstu
    - długość (max 255 bajtów)
      lub oznaczać (w danych MIDa, koniec ciągu przez ustawienie 7 bitu w ostatnim znaku)
*/

//   cmp #$01      ; meta for Text
//   beq pushText
//   cmp #$02      ; meta for Copyright
//   beq pushText
//   cmp #$03      ; meta for Sequence/Track name
//   bne endCase

// pushText:

//   ldx #1
// textCopyLoop:
//   jsr MAIN.MIDFILES.readB
//   sta MAIN.FILESTR.SNULL_ADDR,x
//   dec _tmp
//   beq sendPushText
//   inx
//   cpx #48
//   bne textCopyLoop

// skipRestText:
//   dec _tmp
//   inc _ADR
//   bne checkLen
//   inc _ADR+1
//   jsr memBoundCheck

// checkLen:
//   lda _tmp
//   bne skipRestText

// sendPushText:
//   txa
//   sta MAIN.FILESTR.SNULL_ADDR

//   lda #<MAIN.FILESTR.SNULL_ADDR
//   sta MAIN.MIDFILES.SENDPUSHLCDTEXT.STR
//   lda #>MAIN.FILESTR.SNULL_ADDR
//   sta MAIN.MIDFILES.SENDPUSHLCDTEXT.STR+1

//   jsr sendPushLCDText

//   jmp processTrack

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
