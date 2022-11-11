procedure init;
begin
  DMACTL:=0;
  waitFrame;

  CHBASE:=$32;
  SDLST:=DLIST_ADDR;

  clearWorkArea;
  clearStatus;
  clearUVMeters;
  scradr:=SCREEN_TIME-40; HLine;
  scradr:=SCREEN_TIME-39; putINTText(#$4e+'Status'~*+#$ce);
  scradr:=SCREEN_TIME-10; putINTText(#$4e+'Counter'~*+#$ce);

  MIDTracks:=Pointer(TRACK_DATA_ADDR);

  // for chn:=0 to 15 do
  // begin
  //   // channelScrAdr[chn]:=SCREEN_WORK+42+chn*40;
    // listScrAdr[chn]:=SCREEN_WORK+48+chn*40;
  // end;

  resetMemoryPtr;
  setColors;
  DMACTL:=34; waitFrame;
  otm:=_tm;
  playerStatus:=(playerStatus and %1111) or ps_isStopped;
  // thi:=255;
  // playDir:=1;
  // timeShowMode:=1;
  // SOUNDR:=0;
  // hlpflg:=0;
  // shFile:=0; YFile:=0; curFile:=0;
  // stateInputLine:=0;
  // resultInputLine:=false;

end;