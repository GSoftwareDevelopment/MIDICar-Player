procedure init;
begin
  asm lda #$FE \ sta PORTB end;
  DMACTL:=0;
  waitFrame;

  CHBASE:=hi(CHARS_ADDR)+2; // $32;
  SDLST:=DLIST_ADDR;

  clearWorkArea;
  clearStatus;
  clearUVMeters;
  scradr:=SCREEN_CHANNELS; HLine;
  scradr:=SCREEN_CHANNELS+15; putINTText(#$4e+'Channels'~*+#$ce);
  scradr:=SCREEN_TIME; HLine;
  scradr:=SCREEN_TIME+1; putINTText(#$4e+'Status'~*+#$ce);
  scradr:=SCREEN_TIME+30; putINTText(#$4e+'Counter'~*+#$ce);
  scradr:=SCREEN_OSD+40; putSpaces(20);

  MIDTracks:=Pointer(TRACK_DATA_ADDR);

  // for chn:=0 to 15 do
  // begin
  //   // channelScrAdr[chn]:=SCREEN_WORK+42+chn*40;
  //   listScrAdr[chn]:=SCREEN_WORK+44+chn*40;
  // end;

  setColors;
  calcAvailableMem;
  clearList;
  DMACTL:=34; waitFrame;
  oldLstY:=255;
  initKeyboard;
  setNMI;
  resetTimer(otm); //otm:=_tm;
  playerStatus:=(playerStatus and (ps_calcLen+ps_loop)) or ps_isStopped;
  screenStatus:=screenStatus and (not ss_isOSD+ss_minMode);
  inc(refreshRate);
end;

procedure exit2DOS; keep;
begin
  sendClearPushLCD;
  unsetNMI;
  doneKeyboard;
  doneMIDI;
  Cls(0); Opn(0,12,0,'E:');
  asm
    :2 pla

    pla
    sta PORTB
    jmp main.@halt
  end;
end;