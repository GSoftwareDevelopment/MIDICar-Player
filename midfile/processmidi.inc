{

}

procedure ProcessMIDI;
begin
  if cTrk=totalTracks then
  begin
    cTrk:=1;
    curTrackPtr:=@MIDTracks;
    PlayingTracks:=totalTracks;
  end
  else
  begin
    inc(cTrk);
    inc(curTrackPtr,sizeOf(TMIDTrack));
  end;

  move(curTrackPtr,pointer(_trkRegs),sizeOf(TMIDTrack)); // copy current track data

  if _status and f_stop=0 then
  begin
    if _totalTicks>=_trackTime then ProcessTrack;
  end
  else
    Dec(PlayingTracks);

  move(pointer(_trkRegs),curTrackPtr,sizeOf(TMIDTrack)); // store current track data
end;
