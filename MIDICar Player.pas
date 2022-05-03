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
  Track:PMIDTrack;
  curTrackOfs:Byte;
  trackTime:word;
  dTm:word;
  cTrk,PlayingTracks:Byte;


// {$I mc6850_irq.inc}


procedure reset_MIDI;
begin
  FIFO_Send(GM_Reset,6); FIFO_Flush;
end;

begin
{$IFDEF USE_FIFO}
  FIFO_Reset;
{$ENDIF}

  MIDTracks:=Pointer($4200);
  MIDData:=Pointer($4300);

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

  totalTicks:=0;    // reset song ticks
  setTempo(500000); // set song tempo
  Reset_MIDI;       // reset MIDI

// Player loop
  Write('Playing...');

  Repeat
    PlayingTracks:=nTracks;
    curTrackOfs:=0;
    for cTrk:=1 to nTracks do
    begin
      Track:=@MIDTracks[curTrackOfs];
      inc(curTrackOfs,sizeOf(TMIDTrack));
      if not Track^.EOT then
      begin
        trackTime:=Track^.DeltaTime;
        if totalTicks>=trackTime then
        begin
          _timerStatus:=_timerStatus or f_counter;     // pause ticking
          {$IFDEF DEBUG} poke($d01a,8); {$ENDIF}
          dTm:=totalTicks-trackTime;                   // time correction
          trackTime:=ProcessTrack(Track);
          Track^.deltaTime:=totalTicks+trackTime-dTm;  // time correction
          {$IFDEF DEBUG} poke($d01a,0); {$ENDIF}
          _timerStatus:=_timerStatus and (not f_counter);  // resume ticking
        end;
      end
      else
      begin
        Dec(PlayingTracks); continue;
      end;
    end;

{$IFDEF USE_FIFO}
  _timerStatus:=_timerStatus and (not f_tick);    // reset tick flag
  While FIFO_ReadByte(ZP_Data) do                 // flush FIFO buffer
  begin
    MC6850_Send(ZP_Data);
    if (_timerStatus and f_tick)<>0 then break;   // interrupt immediately, if a new tick occurs
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