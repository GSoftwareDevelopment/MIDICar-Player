{

}
{
procedure ProcessMIDI;
begin
  curTrackPtr:=@MIDTracks;
  PlayingTracks:=totalTracks;
  for cTrk:=1 to totalTracks do
  begin
    move(curTrackPtr,pointer(_trkRegs),sizeOf(TMIDTrack)); // copy current track data

    if _status and f_stop=0 then
    begin
      if _totalTicks>=_trackTime then ProcessTrack;
    end
    else
      Dec(PlayingTracks);

    move(pointer(_trkRegs),curTrackPtr,sizeOf(TMIDTrack)); // store current track data
    inc(curTrackPtr,sizeOf(TMIDTrack));
  end;
end;
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
    ldy #0
    lda (curTrackPtr),y
    and #f_stop
    bne skipProcessTrack

// if _totalTicks>=_trackTime then ProcessTrack;
    ldy #7
    lda _totalTicks+3
    sub (curTrackPtr),y
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
    jmi skipProcessTrack

  // copy current track data to work registers
    ldy #sizeOf_TMIDTrack-1
  loop:
    lda (curTrackPtr),y
    sta _trkRegs,y
    dey
    bpl loop

  end;

  ProcessTrack;

  asm
// store work registers in track data
    ldy #sizeOf_TMIDTrack-1
  loop2:
    lda _trkRegs,y
    sta (curTrackPtr),y
    dey
    bpl loop2

  skipProcessTrack:
  end;

end;
