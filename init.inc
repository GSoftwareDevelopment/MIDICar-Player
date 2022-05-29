procedure MyNMI; Interrupt; Assembler;
asm
  icl 'asms/nmi.a65'
end;

procedure init;
var
  totalXMS:word;

begin
  DMACTL:=0;
{$IFDEF USE_FIFO}
  FIFO_Reset;
{$ENDIF}
  poke(CHBASE,$30);
  clearWorkArea;
  dpoke($230,DLIST_ADDR);
  fn:='';

  scradr:=SCREEN_TIME-28; PutText(#$05+#$12+#$12+#$12+#$12+#$12+'TIME'+#$12+#$12+#$12+#$12+#$12+#$17);
  clearStatus;

  // detect memory
  asm lda #$fe \ sta $100 end;
  totalXMS:=DetectMem;
  totalXMS:=(totalXMS*16)+FREE_MEM;
  scradr:=SCREEN_STATUS+30;
  putText('Mem: ');
  putInt(totalXMS);
  putText('KB');

  MIDTracks:=Pointer(TRACK_DATA_ADDR);
  MIDData:=Pointer(MIDI_DATA_ADDR);

  for cTrk:=0 to 15 do
  begin
    channelScrAdr[cTrk]:=SCREEN_ADDR+120+cTrk*40;
  end;
  oldNMIVec:=NMIVEC;

  NMIEN:=$00; NMIVEC:=@MyNMI; NMIEN:=$40;

  otm:=_tm;
  setColors;
  DMACTL:=34; SOUNDR:=0;
end;