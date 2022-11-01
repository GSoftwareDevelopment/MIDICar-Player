procedure determineSongLength;
var
  event:byte absolute $ff; // yes, this is the same address as `FIFO_Byte` and `MC_Byte` :)

begin
  _songTicks:=0; curTrackPtr:=@MIDTracks;
  for cTrk:=1 to totalTracks do
  begin
    asm
      ldy #sizeOf_TMIDTrack-1
    loop:
      lda (curTrackPtr),y
      sta _trkRegs,y
      dey
      bpl loop
    end;

    asm ldy _bank \ lda $100,y \ sta PORTB end;
    repeat
      readVarL;
      if _delta>0 then
        _trackTime:=_trackTime+_delta;

      if _ptr^ and $80<>0 then
      begin
        ReadB; _event:=event;
      end
      else
        event:=_event;


      if _event<$C0 then
        _tmp:=2 // read 2 bytes
      else if _event<$E0 then
        _tmp:=1 // read 1 byte
      else if event<$F0 then
        _tmp:=2 // read 2 bytes
      else if _event<$F8 then
        readVarL
      else if _event=$ff then
      begin
        readB; // fetch Meta event numer
        readVarL; // fetch data size
        if event=$2f then break; // end of track
      end;
      while _tmp>0 do
      begin
        dec(_tmp);
        inc(_adr);
        memBoundCheck;
      end;
        // while _tmp>0 do
        // begin
        //   readB;
        //   dec(_tmp);
        // end;
    until false;
    if _trackTime>_songTicks then _songTicks:=_trackTime;
    inc(curTrackPtr,sizeOf(TMIDTrack));
  end;
end;