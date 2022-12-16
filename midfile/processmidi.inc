{

}
procedure ProcessMIDI;
begin
{$IFDEF USE_FIFO}
  asm
    ; hook for polling
    ; call driver - flush buffer (set C is hook for send data without empty buffer wait)
    sec
    jsr $2009
  end;
{$ENDIF}
  if _timerStatus and f_counter<>0 then exit;
  if cTrk=totalTracks then
  begin
    cTrk:=1;
    curTrackPtr:=@MIDTracks;
  end
  else
  begin
    inc(cTrk);
    inc(curTrackPtr,sizeOf(TMIDTrack));
  end;

  asm
// if _status and f_stop=0 then
    ldy #6
    lda (curTrackPtr),y
    spl:rts

    // ldy #0
    // lda (curTrackPtr),y
    // and #f_stop
    // beq checkTicks
    // rts
    // bne skipProcessTrack

checkTicks:
// temporary remove f_skipDelta flag
    // lda (curTrackPtr),y
    and #%00111111
    sta tmp

// if _totalTicks>=_trackTime then ProcessTrack;
    lda _totalTicks+3
    sub tmp:#0
    bne L4
    lda _totalTicks+2
    dey
    sub (curTrackPtr),y
    bne L1
    lda _totalTicks+1
    dey
    sub (curTrackPtr),y
    bne L1
    lda _totalTicks
    dey
    sub (curTrackPtr),y
  L1:
    beq L5
    bcs L3
    lda #$ff
    bne L5
  L3:
    lda #$01
    bne L5
  L4:
    bvc L5
    eor #$ff
    ora #$01
  L5:
    jpl doProcessTrack
    rts
    // jmi skipProcessTrack

doProcessTrack:
  // copy current track data to work registers
    ldy #0

    lda (curTrackPtr),y   //  5*
    sta _trkRegs+0        //  3
    iny                   //  2
    lda (curTrackPtr),y   //  5*
    sta _trkRegs+1        //  3
    iny                   //  2
    lda (curTrackPtr),y   //  5*
    sta _trkRegs+2        //  3
    iny                   //  2
    lda (curTrackPtr),y   //  5*
    sta _trkRegs+3        //  3
    iny                   //  2
    lda (curTrackPtr),y   //  5*
    sta _trkRegs+4        //  3
    iny                   //  2
    lda (curTrackPtr),y   //  5*
    sta _trkRegs+5        //  3
    iny                   //  2
    lda (curTrackPtr),y   //  5*
    sta _trkRegs+6        //  3
    iny                   //  2
    lda (curTrackPtr),y   //  5*
    sta _trkRegs+7        //  3
    // iny                   //  2
    // lda (curTrackPtr),y   //  5*
    // sta _trkRegs+8        //  3

                          //  10*8=80

  //   ldy #sizeOf_TMIDTrack-1  // 2
  // loop:
  //   lda (curTrackPtr),y   // 5*
  //   sta _trkRegs,y        // 5
  //   dey                   // 2
  //   bpl loop              // 3*
  //                         // 2 + 8 * 15 = 129
  end;

  ProcessTrack;

  asm
// store work registers in track data
  ldy #0

  lda _trkRegs+0            // 3
  sta (curTrackPtr),y       // 6
  iny                       // 2
  lda _trkRegs+1            // 3
  sta (curTrackPtr),y       // 6
  iny                       // 2
  lda _trkRegs+2            // 3
  sta (curTrackPtr),y       // 6
  iny                       // 2
  lda _trkRegs+3            // 3
  sta (curTrackPtr),y       // 6
  iny                       // 2
  lda _trkRegs+4            // 3
  sta (curTrackPtr),y       // 6
  iny                       // 2
  lda _trkRegs+5            // 3
  sta (curTrackPtr),y       // 6
  iny                       // 2
  lda _trkRegs+6            // 3
  sta (curTrackPtr),y       // 6
  iny                       // 2
  lda _trkRegs+7            // 3
  sta (curTrackPtr),y       // 6
  // iny                       // 2
  // lda _trkRegs+8            // 3
  // sta (curTrackPtr),y       // 6
                            // 8 * 11 = 88
  //   ldy #sizeOf_TMIDTrack-1
  // loop2:
  //   lda _trkRegs,y        // 4*
  //   sta (curTrackPtr),y   // 6
  //   dey                   // 2
  //   bpl loop2             // 3*
                             // = 2 + 15 * 9 = 137

  // skipProcessTrack:
  end;

end;
