{$DEFINE ROMOFF}
Uses
  MC6850,
  MIDI_FIFO,
  MIDFiles;
{$I-}

const
  GM_RESET: array[0..5] of byte = ($f0, $7e, $7f, $09, $01, $f7);

var
  fn:PString;
  trkPtr:PMIDTrack;
  curTrackOfs:Byte;
  deltaTime:TDeltaTime;
  dTm:word;
  cTrk,PlayingTracks:Byte;


// {$I mc6850_irq.inc}


procedure reset_MIDI;
begin
  FIFO_Send(GM_Reset,6); FIFO_Flush;
end;

begin
{$IFDEF DEBUG}
  WriteLn('Debug mode is On');
{$ENDIF}
{$IFDEF USE_FIFO}
  WriteLn('FIFO: On');
  FIFO_Reset;
{$ELSE}
  WriteLn('FIFO: Off');
{$ENDIF}

  MIDTracks:=Pointer($4000);
  MIDData:=Pointer($4100);

  if paramCount=1 then
    fn:=ParamStr(1)
  else
  begin
      // writeLn('Use: P device:filename.ext');
      // halt(0);
    fn:='D2:SELFTEST.MID';
  end;

  if not LoadMID(fn) then halt(1);

//

  // initialize MIDICar (MC6850)
  MC6850_Reset;
  MC6850_Init(CD_64+WS_OddParity+WS_8bits+IRQ_Receive);

  // initialize IRQ for MC6850
  // oldIRQ:=VIMIRQ; asm SEI end; VIMIRQ:=@MC6850_IRQ; asm CLI end;

  Write('Playing...');

  Reset_MIDI;        // reset MIDI
  _totalTicks:=0;    // reset song ticks
  // setIntVec(iTim1,@int_play,0,255);
  setTempo;          // set song tempo

// Player loop

  cTrk:=nTracks;
  Repeat

    if cTrk=nTracks then
    begin
      curTrackOfs:=0; cTrk:=1;
      PlayingTracks:=nTracks;
    end
    else
    begin
      inc(curTrackOfs,sizeOf(TMIDTrack));
      inc(cTrk);
    end;

    move(@MIDTracks[curTrackOfs],pointer(_trkRegs),sizeOf(TMIDTrack)); // copy current track data

    if _trackTime>=0 then
    begin
      if _totalTicks>=_trackTime then
      begin
        _timerStatus:=_timerStatus or f_counter;     // pause ticking
        {$IFDEF DEBUG} poke($d01a,15); {$ENDIF}
        dTm:=_totalTicks-_trackTime;                 // time correction
        deltaTime:=ProcessTrack;
        if deltaTime>0 then
          _trackTime:=_totalTicks+deltaTime-dTm      // calculate new track time with time correction
        else
          _trackTime:=-1;
        {$IFDEF DEBUG} poke($d01a,0); {$ENDIF}
        _timerStatus:=_timerStatus and (not f_counter);  // resume ticking
      end;
    end
    else
      Dec(PlayingTracks);
    move(pointer(_trkRegs),@MIDTracks[curTrackOfs],sizeOf(TMIDTrack)); // copy current track data

{$IFDEF USE_FIFO}
    if (MC6850_CNTRReg and TDRE)<>0 then
      if FIFO_Tail<>FIFO_Head then
      begin
{$IFDEF DEBUG} poke($d01a,4); {$ENDIF}
        FIFO_ReadByte;
        MC6850_BUFFER:=FIFO_Byte;
{$IFDEF DEBUG} poke($d01a,0); {$ENDIF}
      end;
{$ENDIF}

  until (PlayingTracks=0) or (peek(764)<>255);

  Write(#156);

// End player loop

  // poke(559,34);
  // dpoke($9c24,$9c40);

  Reset_MIDI;

  // restore IRQ vector
  // asm SEI end; VIMIRQ:=oldIRQ; asm CLI end;

  // resotre Timer vector
  setIntVec(iTim1,oldTimerVec);
end.