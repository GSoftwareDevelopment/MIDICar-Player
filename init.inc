procedure init;

  procedure setMCBaseAddress; assembler;
  asm
    icl 'asms/setMCBaseAddr.a65'
  end;

begin
  setMCBaseAddress;
  // initialize MIDICar (MC6850)

  MC6850_Init(MasterReset);
  MC6850_Init(CD_64+WS_OddParity+WS_8bits+WS_2BitStop{+IRQ_Receive});

  DMACTL:=0;
  CHBASE:=$32;
  SDLST:=DLIST_ADDR;
{$IFDEF USE_FIFO}
  FIFO_Reset;
{$ENDIF}
  fn:=paramstr(0); getDeviceSpec(fn,curDev);
  if curDev='' then curDev:='D:';
  fn:='';

  clearWorkArea;
  clearStatus;
  scradr:=SCREEN_TIME-40; HLine;
  scradr:=SCREEN_TIME-39; putINTText(#$4e+'Status'~*+#$ce);
  scradr:=SCREEN_TIME-10; putINTText(#$4e+'Counter'~*+#$ce);

  MIDTracks:=Pointer(TRACK_DATA_ADDR);
  // MIDData:=Pointer(MIDI_DATA_ADDR);

  for cTrk:=0 to 15 do
  begin
    channelScrAdr[cTrk]:=SCREEN_WORK+42+cTrk*40;
    playlistScrAdr[cTrk]:=SCREEN_WORK+48+cTrk*40;
  end;

  asm
    lda $14
    cmp $14
    beq *-2
  end;

  resetMemoryPtr;
  otm:=_tm;
  setColors; DMACTL:=34; SOUNDR:=0;
  isStopped:=True;
end;