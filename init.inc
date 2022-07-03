procedure init;

  procedure setMCBaseAddress; assembler;
  asm
    icl 'asms/setMCBaseAddr.a65'
  end;

begin
  DMACTL:=0;
  asm
    lda $14
    cmp $14
    beq *-2
  end;
  setMCBaseAddress;
  // initialize MIDICar (MC6850)

  MC6850_Init(MasterReset);
  MC6850_Init(CD_64+WS_OddParity+WS_8bits+WS_2BitStop{+IRQ_Receive});

  CHBASE:=$32;
  SDLST:=DLIST_ADDR;
{$IFDEF USE_FIFO}
  FIFO_Reset;
{$ENDIF}
  curDev:='D:';
  fn:='';

  clearWorkArea;
  clearStatus;
  scradr:=SCREEN_TIME-40; HLine;
  scradr:=SCREEN_TIME-39; putINTText(#$4e+'Status'~*+#$ce);
  scradr:=SCREEN_TIME-10; putINTText(#$4e+'Counter'~*+#$ce);

  MIDTracks:=Pointer(TRACK_DATA_ADDR);

  // for chn:=0 to 15 do
  // begin
  //   // channelScrAdr[chn]:=SCREEN_WORK+42+chn*40;
  //   listScrAdr[chn]:=SCREEN_WORK+48+chn*40;
  // end;

  resetMemoryPtr;
  otm:=_tm;
  setColors; DMACTL:=34; SOUNDR:=0; playDir:=1;
  playerStatus:=(playerStatus and %1111) or ps_isStopped;
  thi:=255; hlpflg:=0;
  stateInputLine:=0;
  resultInputLine:=false;
end;