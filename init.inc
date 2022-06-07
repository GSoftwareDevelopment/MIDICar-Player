procedure init;
var
  memAvailable:word;

  procedure setMCBaseAddress; assembler;
  asm
    .macro m@setMCBase name
      sty :name
      stx :name+1
    .endm

    ldy $d8
    ldx $d9

    m@setMCBase MAIN.MC6850.MC6850_INIT.MCBASESTATE
    m@setMCBase MAIN.MC6850.MC6850_Send2.MCBaseState
    m@setMCBase MAIN.MIDI_FIFO.FIFO_PUSHDIRECT2MC6850.MCBaseState
    m@setMCBase MAIN.MIDI_FIFO.FIFO_FLUSH.MCBaseState

    iny
    m@setMCBase MAIN.MC6850.MC6850_Send2.MCBaseBuf
    m@setMCBase MAIN.MIDI_FIFO.FIFO_PUSHDIRECT2MC6850.MCBaseBuf
    m@setMCBase MAIN.MIDI_FIFO.FIFO_FLUSH.MCBaseBuf
  end;

begin
  setMCBaseAddress;
  // initialize MIDICar (MC6850)

  MC6850_Init(MasterReset);
  MC6850_Init(CD_64+WS_OddParity+WS_8bits+WS_2BitStop{+IRQ_Receive});

  DMACTL:=0;
{$IFDEF USE_FIFO}
  FIFO_Reset;
{$ENDIF}

  poke(CHBASE,$32);
  fillchar(pointer(SCREEN_WORK),SCREEN_TIME-SCREEN_WORK,0);
  dpoke($230,DLIST_ADDR);
  fn:='';

  scradr:=SCREEN_TIME-29; PutINTText(#$46+#$4a+#$4a+#$4a+#$4a+#$4a+#$c8+'TIME'*~+#$c7+#$4a+#$4a+#$4a+#$4a+#$4a+#$49);
  clearStatus;

  // detect memory
  asm lda #$fe \ sta $100 end;
  totalXMS:=DetectMem;
  memAvailable:=(word(totalXMS)*16)+FREE_MEM;
  scradr:=SCREEN_STATUS+30;
  putINTText('Mem:'~);
  putInt(memAvailable);
  putINTText('KB'~);

  MIDTracks:=Pointer(TRACK_DATA_ADDR);
  MIDData:=Pointer(MIDI_DATA_ADDR);

  for cTrk:=0 to 15 do
    channelScrAdr[cTrk]:=SCREEN_WORK+cTrk*40;

  asm
    lda $14
    cmp $14
    beq *-2
  end;

  otm:=_tm;
  setColors; DMACTL:=34; // SOUNDR:=0;
  isStopped:=True;
end;